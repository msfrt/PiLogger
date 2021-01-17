/**
 * \file ThreadableMsgQueue.h
 *
 * \author Dave Yonkers
 *
 * Class that implements a circular queue that can be safely shared between threads
 */

#pragma once

#include <semaphore.h>
#include <vector>
#include <memory>

class CReceivedMessage;

/**
 * Class that implements a circular queue that can be safely shared between threads
 *
 * This class uses semaphores to block threads if the critical section is currently
 * in use (when a message is being inserted), if there are no empty slots in the
 * queue array (when attempting to insert), or if there are no messages to consume
 * (when requesting to pop a queue that's empty)
 */
template <class queue_item_type>
class CThreadableMsgQueue {

public:

    /** Default constructor */
    CThreadableMsgQueue() {}

    /** Copy constructor disabled */
    CThreadableMsgQueue(const CThreadableMsgQueue &) = delete;
    /** Assignment operator disabled */
    void operator=(const CThreadableMsgQueue &) = delete;

    void SetBufferSize(int size);
    int Initialize();

    void Push(queue_item_type item);
    queue_item_type Pop();

private:

    // these are the semaphores that will control the buffer access
    sem_t mS;  /// buffer mutual exclusion
    sem_t mN;  /// for the number of items currently in the buffer
    sem_t mE;  /// for the number of empty slots

    /// buffer that will hold the queue items
    std::vector<queue_item_type> mBuffer;

    int mItemsK   = 5000;  /// default buffer size
    int mItemsIn  = 0;     /// buffer index of the next insertable location
    int mItemsOut = 0;     /// buffer index of the next popable location

    int mInitialized = false; /// to prevent unauthorized config after queue has been initialized

};


