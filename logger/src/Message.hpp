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

    /**
     * Sets the bus' name
     * \param bus A pointer to the bus' string name (ptr to save copies)
     */
    void SetBusName(std::string *bus) { mBusName = bus; }

    /**
     * Get the bus name
     * \returns A pointer to the bus' name string
     */
    std::string *GetBusName() { return mBusName; }


    /**
     * Set this message's time
     */ 
    void Timestamp() {
        std::time(&mTime);
    }

    /**
     * Gets the time that the message was logged at
     * \returns The time_t of the logging timestamp
     */  
    time_t GetTime() { return mTime; }


private:

    std::string *mBusName = nullptr;   /// the name of the bus, ex. vcan0
    struct can_frame *mFrame;   /// the actual frame recorded
    time_t mTime;   /// The time that the message was recieved
    

};


