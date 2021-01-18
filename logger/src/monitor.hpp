/**
 * \file monitor.hpp
 *
 * \author Dave Yonkers
 * 
 * This is simply the definition of the CAN socket monitor function.
 * This function will be the "main" function of the socket monitoring threads.
 */

#include <string>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "Message.hpp"
#include "ThreadableQueue.hpp"

/**
 * This struct should be populated and passed to the monitor thread as a void*
 * object. It will then be downcast (oof) to obtain the parameters
 */
struct MonitorParams {
    std::string iface_name;
    CThreadableMsgQueue<CMessage> *queue;
};

void* monitor(void*);

