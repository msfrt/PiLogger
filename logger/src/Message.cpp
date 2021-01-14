/**
 * \file Message.cpp
 *
 * \author Dave Yonkers
 */

#include "Message.hpp"

#include <sys/time.h>
#include <ctime>

/**
 * Constructor for a message
 * \param bus A pointer to the bus' name string
 */
CMessage::CMessage(std::string *bus) : mBusName(bus) {
    mTime = time(nullptr);
}


/**
 * Destructor for a message
 * This frees the memory that was allocated for the CAN frame
 */
CMessage::~CMessage() {
    delete mFrame;
}

