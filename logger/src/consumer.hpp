/**
 * \file consumer.hpp
 *
 * \author Dave Yonkers
 * 
 * This thread will consume the CAN messages
 */

#include <string>
#include <unordered_map>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "Message.hpp"
#include "ThreadableQueue.hpp"
#include "interfaces.hpp"


struct DBInfo {
    std::string host;
    int port;
    std::string org;
    std::string bucket;
    std::string token;
};


/**
 * This struct should be populated and passed to the monitor thread as a void*
 * object. It will then be downcast (oof) to obtain the parameters
 */
struct ConsumerParams {
    std::unordered_map<Interface, std::string> bus_dbc_file_map;
    CThreadableMsgQueue<CMessage*> *queue;
    DBInfo dbinfo;
};

void* consumer(void*);

