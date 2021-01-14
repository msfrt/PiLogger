/**
 * \file ThreadableMsgQueue.cpp
 *
 * \author Dave Yonkers
 */

#include "ThreadableMsgQueue.hpp"

/**
 * Set the size of the queue before initializing the semaphores
 * \param size The size of the queue buffer
 */
void CThreadableMsgQueue::SetBufferSize(int size) {
    if (!mInitialized)
        mMessagesK = size;
}

/**
 * Initialize the semaphores and the queue buffer if they
 * haven't already been.
 * \returns 0 on success, -1 on failure (program should exit)
 */
int CThreadableMsgQueue::Initialize() {
    if (!mInitialized){

        // initialize semaphores
        if (sem_init(&mS, 0, 1) == -1)
            return -1;

        if (sem_init(&mN, 0, 0) == -1) 
            return -1;

        if (sem_init(&mE, 0, mMessagesK) == -1) 
            return -1;

        // resize buffer
        mMessages.resize(mMessagesK);

        mInitialized = true;
    }

    return 0;
}

