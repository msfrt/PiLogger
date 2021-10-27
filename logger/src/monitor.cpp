/**
 * \file monitor.cpp
 *
 * \author Dave Yonkers
 */

#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>

#include <memory>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <semaphore.h>

#include "Message.hpp"
#include "ThreadableQueue.hpp"

#include "monitor.hpp"

using namespace std;

extern const bool LOGGER_DEBUG;
extern bool stop_logging;
extern sem_t stdout_sem;


/**
 * A simple function to gracefully close a socket. I made this a function so that
 * it'd be easy to close a socket in more than one place.
 * \param socket_num The index of the socket to close
 */
void CloseSocket(int socket_num){
    if (close(socket_num) < 0) {
		cerr << "ERROR: Problem closing socket ID " << socket_num << endl;
        return;
	}
    if (LOGGER_DEBUG) cout << "Successfully closed socket ID " << socket_num << endl;
}


/**
 * CAN socket monitoring thread main function.
 * 
 * This thread simply monitors the CAN socket. When a message is recieved, it's
 * thrown into a CMessage, which records the time and the interface name. The
 * CMessage is then thrown into the decoding queue for further processing.
 */
void* monitor(void* args){

    // cast params pointer to a useable form (oooof, Dr. Owen would hate me)
    MonitorParams params = *(MonitorParams*) args;


    // open a socket
    int s;
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (LOGGER_DEBUG)
        cout << "Socket created for " << iface_to_string(params.iface_name) << " with return value: " << s << endl;
    if (s < 0) {
        cerr << "ERROR: could not open socket for " << params.iface_name << "!" << endl;
        return (void*)1;
    }


    // interface name and socket union
    struct ifreq ifr;
    string iface_str = iface_to_string(params.iface_name);
    strncpy(ifr.ifr_name, iface_str.c_str(), iface_str.length() + 1);
	if (ioctl(s, SIOCGIFINDEX, &ifr) < 0){
        cerr << "ERROR: could not join " << iface_to_string(params.iface_name) << " to socket ID " << 
            s << "! Result: " << ifr.ifr_name << endl;
        CloseSocket(s);
        return (void*)1;
    }


    // bind the socket to the CAN interface index
    struct sockaddr_can addr;

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        cerr << "ERROR: could not bind socket " << s << " to interface " << 
            iface_to_string(params.iface_name) << " (ifindex " << ifr.ifr_ifindex << ")" << endl;
        CloseSocket(s);
        return (void*)1;
    }


    // loop forever (or until asked to stop)
    while (!stop_logging){

        int nbytes;

	    //struct can_frame *frame = new struct can_frame;  // allocate a new CAN frame (avoids needless copies)
        shared_ptr<can_frame> frame = make_shared<can_frame>();

        // read a CAN frame from the socket. This function call is blocking!
        nbytes = read(s, frame.get(), sizeof(struct can_frame));



        shared_ptr<CMessage> msg = make_shared<CMessage>();  // create a message from this bus
        msg->Timestamp();  // record the time
        msg->SetFrame(frame);  // set the frame ptr
        msg->SetBusName(params.iface_name);  // set the bus name
        params.queue->Push(msg);  // launch it into the queue!
        

        if (false){
            sem_wait(&stdout_sem);
            cout << iface_to_string(params.iface_name)  << "  monitor - ";
            cout << hex << setw(3) << setfill('0') << frame->can_id << ": ";
            for (int i = 0; i < frame->can_dlc; i++)
                printf("%02X ",frame->data[i]);
            cout << endl;
            sem_post(&stdout_sem);
        }

    }


    // close the socket
    CloseSocket(s);

    return nullptr;
}






