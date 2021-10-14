/**
 * \file consumer.cpp
 *
 * \author Dave Yonkers
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <memory>

#include <string.h>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <semaphore.h>

#include <dbcppp/Network.h>

#include "Message.hpp"
#include "ThreadableQueue.hpp"

#include "consumer.hpp"

#include "../src/external/influxdb-cpp-2/influxdb.hpp"

#include <chrono>

using namespace std;
using namespace influxdb_cpp;

extern const bool LOGGER_DEBUG;
extern bool stop_logging;
extern sem_t stdout_sem;


/**
 * Helper functions go here!
 */
namespace csmr {

    /**
     * Gets the current system unix time in milliseconds
     * \returns The time in milliseconds
     */
    long int current_time(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

}



/**
 * CAN socket consuming thread main function.
 * 
 * This thread pops a CAN frame from the decoding queue whenever it is available,
 * decodes it, then adds it to a batch write for the database.
 */
void* consumer(void* args){

    // cast params pointer to a useable form (oooof, Dr. Owen would hate me)
    ConsumerParams params = *(ConsumerParams*) args;

    auto queue = params.queue;
    auto &dbc_names_map = params.bus_dbc_file_map;
    auto dbinfo = params.dbinfo;


    // load the DBC files -----

    // maps interface name (can0/1) to dbc network object
    std::unordered_map<Interface, std::unique_ptr<dbcppp::INetwork>> dbc_map;

    // maps interface name (can0/1) to messages within that DBC (for fast lookups)
    std::unordered_map<Interface, std::unordered_map<uint64_t, const dbcppp::IMessage*>> msgs_map;

    if (LOGGER_DEBUG) {
        sem_wait(&stdout_sem);
        cout << "[DECODER]: Loading DBCs..." << endl;
        sem_post(&stdout_sem);
    }

    for (auto pair : dbc_names_map) {

        // open the DBC file and check to see that is indeed an actual file
        ifstream dbc_file;
        dbc_file.open(pair.second.c_str());
        if (!dbc_file) {
            dbc_map[pair.first] = nullptr;  // no file

            if (LOGGER_DEBUG) {
                sem_wait(&stdout_sem);
                cout << "[DECODER]: Could not load DBC for " << iface_to_string(pair.first) << endl;
                sem_post(&stdout_sem);
            }

            continue;
        }

        // create the DBC network object
        std::unique_ptr<dbcppp::INetwork> net = dbcppp::INetwork::LoadDBCFromIs(dbc_file);

        // insert the message pointers into the unordered map for O(1) lookup times with key value
        // pair (ID, *message)
        for (const dbcppp::IMessage& msg : net->Messages())
        {
            msgs_map[pair.first].insert(std::make_pair(msg.Id(), &msg));
        }

        // move the unique ptr to the bus map
        dbc_map[pair.first] = std::move(net);

    }

    if (LOGGER_DEBUG) {
        sem_wait(&stdout_sem);
        cout << "[DECODER]: DBCs loading complete!" << endl;
        sem_post(&stdout_sem);
    }


    // pack the database server info into the server info object
    server_info si(dbinfo.host, dbinfo.port, dbinfo.org, dbinfo.token, dbinfo.bucket);


    // initialize a variable that will hold the next time that we're forced to write to
    // the database.
    auto next_write_time = csmr::current_time() + params.max_write_delay;
    
    // variable to keep track of the number of measurements in the current batch
    int current_batch_size = 0;


    // decode messages until asked to stop -----
    while (!stop_logging){


        // loop to read and package a batch write
        while ((csmr::current_time() <= next_write_time) && !stop_logging){

            

            // pop a CMessage from the queue
            auto message = queue->Pop();
            auto frame = message->GetFrame();
            auto iface = message->GetBusName();

            // cout << frame->can_id << " - " << frame->data[0] << endl;
            

            // print the frame data if in debug mode
            if (false){
                sem_wait(&stdout_sem);
                cout << iface_to_string(iface) << " consumer - ";
                cout << hex << setw(3) << setfill('0') << frame->can_id << ": ";
                for (int i = 0; i < frame->can_dlc; i++)
                    printf("%02X ",frame->data[i]);
                cout << endl;
                sem_post(&stdout_sem);
            }

            // cout << "iface yuh: " << iface << " with ID " << hex << frame->can_id << dec << endl;
            // const dbcppp::Message* msg = dbc_map[iface]->getMessageById(frame->can_id);
            // if (msg){
            //     std::cout << "Received Message: " << msg->getName() << " from " << iface_to_string(iface) << endl;
            // }

            // if there was a DBC successfully loaded for this interface
            if (dbc_map[iface]){

                // find the message within the msgs_map for this bus
                auto iter = msgs_map[iface].find(frame->can_id);
                if (iter != msgs_map[iface].end()){  // if we found a corresponding message
                    const dbcppp::IMessage* msg = iter->second;

                    std::cout << "Received Message: " << msg->Name() << " from " << iface_to_string(iface) << endl;

                    // Iterate through message's signals
                    for (const dbcppp::ISignal& sig : msg->Signals()) {
                        const dbcppp::ISignal* mux_sig = msg->MuxSignal();

                        if (sig.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue ||
                            (mux_sig && mux_sig->Decode(frame->data) == sig.MultiplexerSwitchValue())) {

                            std::cout << "\t" << sig.Name() << "=" << sig.RawToPhys(sig.Decode(frame->data)) << sig.Unit() << "\n";
                        }
                    }

                    
                }




            } // end if (dbc_map[iface])

            // free the can frame and the message
            delete message->GetFrame();
            delete message;

        }

        // set the next time to write a batch and reset the batch size count
        next_write_time = csmr::current_time() + params.max_write_delay;
        current_batch_size = 0;






    }

    return nullptr;
}






