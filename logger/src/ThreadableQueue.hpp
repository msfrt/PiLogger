/**
 * \file ThreadableMsgQueue.h
 *
 * \author Dave Yonkers
 *
 * Class that implements a circular queue that can be safely shared between threads.
 * This is contained all in one file because it is templated.
 */

#pragma once

#include <semaphore.h>
#include <vector>
#include <memory>

#include "Message.hpp"


/**
 * Class that implements a circular queue that can be safely shared between threads
 *
 * This class uses semaphores to block threads if the critical section is currently
 * in use (when a message is being inserted), if there are no empty slots in the
 * queue array (when attempting to insert), or if there are no messages to consume
 * (when requesting to pop a queue that's empty)
 */
template <class queue_type>
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

    /**
     * Getter for the buffer size
     * \returns The buffer size in slots
     */ 
    int GetBufferSize() const { return mItemsK; }

    /**
     * Getter for the number of items current in the queue
     * \returns The number of items in the queue
     */
    int GetLength() { int i; sem_getvalue(&mN, &i); return i; }

    void Push(queue_type item);
    queue_type Pop();

private:

    // these are the semaphores that will control the buffer access
    sem_t mS;  /// buffer mutual exclusion
    sem_t mN;  /// for the number of items currently in the buffer
    sem_t mE;  /// for the number of empty slots

    /// buffer that will hold the queue items
    std::vector<queue_type> mBuffer;

    int mItemsK   = 5000;  /// default buffer size
    int mItemsIn  = 0;     /// buffer index of the next insertable location
    int mItemsOut = 0;     /// buffer index of the next popable location

    int mInitialized = false; /// to prevent unauthorized config after queue has been initialized

};


/**
 * Set the size of the queue before initializing the semaphores
 * \param size The size of the queue buffer
 */
template <class queue_type>
void CThreadableMsgQueue<queue_type>::SetBufferSize(int size) {
    if (!mInitialized)
        mItemsK = size;
}


/**
 * Initialize the semaphores and the queue buffer if they
 * haven't already been.
 * \returns 0 on success, -1 on failure (program should exit)
 */
template <class queue_type>
int CThreadableMsgQueue<queue_type>::Initialize() {
    if (!mInitialized){

        // initialize semaphores
        if (sem_init(&mS, 0, 1) == -1)
            return -1;

        if (sem_init(&mN, 0, 0) == -1) 
            return -1;

        if (sem_init(&mE, 0, mItemsK) == -1) 
            return -1;

        // resize buffer
        mBuffer.resize(mItemsK);

        mInitialized = true;
    }

    return 0;
}


/**
 * Push an item into the queue... safely
 * \param item The item to push onto the top of the queue
 */
template <class queue_type>
void CThreadableMsgQueue<queue_type>::Push(queue_type item) {

    if (!mInitialized)
        Initialize();

    // insert the transaction into the buffer
    sem_wait(&mE);
    sem_wait(&mS);
    mBuffer.at(mItemsIn) = item;
    mItemsIn = (mItemsIn + 1) % mItemsK;
    sem_post(&mS);
    sem_post(&mN);

}



/**
 * Pop an item from the queue... safely
 * \returns The item that was popped from the bottom of the queue
 */
template <class queue_type>
queue_type CThreadableMsgQueue<queue_type>::Pop() {

    if (!mInitialized)
        Initialize();

    queue_type item;

    sem_wait(&mN);
    sem_wait(&mS);

    // get the next item from the queue
    item = mBuffer.at(mItemsOut);
    mItemsOut = (mItemsOut + 1) % mItemsK;

    sem_post(&mS);
    sem_post(&mE);

    return item;
}