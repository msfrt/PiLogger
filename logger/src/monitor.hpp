/**
 * \file monitor.hpp
 *
 * \author Dave Yonkers
 * 
 * This is simply the definition of the CAN socket monitor function.
 * This function will be the "main" function of the socket monitoring threads.
 */

#include <string>

class CThreadableMsgQueue;

/**
 * This struct should be populated and passed to the monitor thread as a void*
 * object. It will then be cast (oof) to obtain the parameters
 */
struct MonitorParams {
    std::string iface_name;
    CThreadableMsgQueue *queue;
};

void* monitor(void*);

