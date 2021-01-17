/**
 * \file ThreadableMsgQueue.cpp
 *
 * \author Dave Yonkers
 */

#include <semaphore.h>

#include "ThreadableMsgQueue.hpp"


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
