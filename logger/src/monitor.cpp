/**
 * \file monitor.cpp
 *
 * \author Dave Yonkers
 */


/**
 * CAN socket monitoring thread main function.
 * 
 * This thread simply monitors the CAN socket. When a message is recieved, it's
 * thrown into a CMessage, which records the time and the interface name. The
 * CMessage is then thrown into the decoding queue for further processing.
 */
void* monitor(void*){

    for (long i; i < 100000000; i++){
        
    }

    return nullptr;
}


