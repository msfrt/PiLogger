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

#include <chrono>
#include <sys/time.h>
#include <ctime>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "interfaces.hpp"

/**
 * Class that simply holds a message frame, a bus indicator, and a timestamp
 */
class CMessage {

public:

    // /**
    //  * Set a pointer to the CAN frame
    //  * \param frame A pointer to the frame
    //  */
    // void SetFrame(can_frame *frame) { mFrame = frame; }

    // /** 
    //  * Gets a pointer to the CAN frame
    //  * \returns A pointer to the CAN frame
    //  */
    // can_frame* GetFrame() { return mFrame; }

    /**
     * Set a pointer to the CAN frame
     * \param frame A shared pointer to the frame
     */
    void SetFrame(std::shared_ptr<can_frame> frame) { mFrame = frame; }

    /** 
     * Gets a pointer to the CAN frame
     * \returns A shared pointer to the CAN frame
     */
    std::shared_ptr<can_frame> GetFrame() { return mFrame; }

    /**
     * Sets the bus' name
     * \param bus An interface enumeration beloning to the bus
     */
    void SetBusName(Interface iface) { mBusName = iface; }

    /**
     * Get the bus name
     * \returns A the interface enum
     */
    Interface GetBusName() { return mBusName; }


    /**
     * Set this message's time
     */ 
    void Timestamp() {
        mTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    }

    /**
     * Gets the time that the message was logged at
     * \returns The long of the logging timestamp
     */  
    unsigned long long GetTime() { return mTime.count() * 1000000; }


private:

    Interface mBusName;   /// the name of the bus, ex. vcan0
    //struct can_frame *mFrame;   /// the actual frame recorded
    std::shared_ptr<can_frame> mFrame;
    std::chrono::milliseconds mTime;   /// The time that the message was recieved

};


