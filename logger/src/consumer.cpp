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

using namespace std;

extern const bool LOGGER_DEBUG;
extern bool stop_logging;
extern sem_t stdout_sem;



/**
 * CAN socket monitoring thread main function.
 * 
 * This thread simply monitors the CAN socket. When a message is recieved, it's
 * thrown into a CMessage, which records the time and the interface name. The
 * CMessage is then thrown into the decoding queue for further processing.
 */
void* consumer(void* args){

    // cast params pointer to a useable form (oooof, Dr. Owen would hate me)
    ConsumerParams params = *(ConsumerParams*) args;

    auto queue = params.queue;
    auto &dbc_names_map = params.bus_dbc_file_map;


    std::unordered_map<Interface, std::unique_ptr<dbcppp::Network>> dbc_map;

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
        std::unique_ptr<dbcppp::Network> net = dbcppp::Network::loadDBCFromIs(dbc_file);

        // move the unique ptr to the bus map
        dbc_map[pair.first] = std::move(net);
        

    }


    if (LOGGER_DEBUG) {
        sem_wait(&stdout_sem);
        cout << "[DECODER]: DBCs loading complete!" << endl;
        sem_post(&stdout_sem);
    }
    


    while (!stop_logging){


        // pop a CMessage from the queue
        auto message = queue->Pop();
        auto frame = message->GetFrame();
        auto iface_name = message->GetBusName();
        

        if (LOGGER_DEBUG){
            sem_wait(&stdout_sem);
            cout << iface_to_string(iface_name) << " consumer - ";
            cout << hex << setw(3) << setfill('0') << frame->can_id << ": ";
            for (int i = 0; i < frame->can_dlc; i++)
                printf("%02X ",frame->data[i]);
            cout << endl;
            sem_post(&stdout_sem);
        }




        // free the can frame and the message
        delete message->GetFrame();
        delete message;

    }

    return nullptr;
}





