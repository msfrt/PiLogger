/**
 * \file consumer.cpp
 *
 * \author Dave Yonkers
 */

#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <semaphore.h>

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


    while (!stop_logging){


        // pop a CMessage from the queue
        auto message = queue->Pop();
        auto frame = message->GetFrame();
        auto iface_name = message->GetBusName();
        

        if (LOGGER_DEBUG){
            sem_wait(&stdout_sem);
            cout << *iface_name << " consumer - ";
            cout << hex << setw(3) << setfill('0') << frame->can_id << ": ";
            for (int i = 0; i < frame->can_dlc; i++)
                printf("%02X ",frame->data[i]);
            cout << endl;
            sem_post(&stdout_sem);
        }


    }

    return nullptr;
}






