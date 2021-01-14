/**
 * \file Message.hpp
 *
 * \author Dave Yonkers
 *
 * Class that simply holds a message frame, a bus indicator, and a timestamp
 */

#pragma once

#include <memory>
#include <string>

#include <sys/time.h>
#include <ctime>

#include <linux/can.h>
#include <linux/can/raw.h>

/**
 * Class that simply holds a message frame, a bus indicator, and a timestamp
 */
class CMessage {

public:

    /** Default constructor */
    CMessage() = delete;

    /** Copy constructor disabled */
    CMessage(const CMessage &) = delete;
    /** Assignment  operator disabled */
    void operator=(const CMessage &) = delete;

    CMessage(std::string *bus);
    ~CMessage();

    /**
     * Set a pointer to the CAN frame
     * \param frame A pointer to the frame
     */
    void SetFrame(can_frame *frame) { mFrame = frame; }

    /** 
     * Gets a pointer to the CAN frame
     * \returns A pointer to the CAN frame
     */
    can_frame* GetFrame() { return mFrame; }


private:

    std::string *mBusName = nullptr;   /// the name of the bus, ex. vcan0
    struct can_frame *mFrame;   /// the actual frame recorded
    time_t mTime;   /// The time that the message was recieved
    

};


