/**
 * \file monitor.cpp
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
        cout << "Socket created for " << params.iface_name << " with return value: " << s << endl;
    if (s < 0) {
        cerr << "ERROR: could not open socket for " << params.iface_name << "!" << endl;
        return (void*)1;
    }


    // interface name and socket union
    struct ifreq ifr;
    strncpy(ifr.ifr_name, params.iface_name.c_str(), params.iface_name.length() + 1);
	if (ioctl(s, SIOCGIFINDEX, &ifr) < 0){
        cerr << "ERROR: could not join " << params.iface_name << " to socket ID " << 
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
            params.iface_name << " (ifindex " << ifr.ifr_ifindex << ")" << endl;
        CloseSocket(s);
        return (void*)1;
    }


    // loop forever (or until asked to stop)
    while (!stop_logging){

        int nbytes;
	    struct can_frame frame;

        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (LOGGER_DEBUG){
            sem_wait(&stdout_sem);
            cout << hex << setw(3) << setfill('0') << frame.can_id << ": ";
            for (int i = 0; i < frame.can_dlc; i++)
                printf("%02X ",frame.data[i]);
            cout << endl;
            sem_post(&stdout_sem);
        }

    }


    // close the socket
    CloseSocket(s);

    return nullptr;
}






