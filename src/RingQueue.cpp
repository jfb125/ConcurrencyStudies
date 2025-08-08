/*
 * RingQueue.cpp
 *
 *  Created on: Aug 8, 2025
 *      Author: joe
 */

#include "RingQueue.h"

RingQueue::RingQueue() {
    // TODO Auto-generated constructor stub

}

RingQueue::~RingQueue() {
    // TODO Auto-generated destructor stub
}

RingQueue::RingQueue(const RingQueue &other) {
    // TODO Auto-generated constructor stub

}

RingQueue::RingQueue(RingQueue &&other) {
    // TODO Auto-generated constructor stub

}

RingQueue& RingQueue::operator=(const RingQueue &other) {
    // TODO Auto-generated method stub

}

RingQueue& RingQueue::operator=(RingQueue &&other) {
    // TODO Auto-generated method stub

}

void RingQueue::reset();
bool RingQueue::enqueue(T* &p_object);
T*   RingQueue::dequeue();
bool RingQueue::isError() const;
RingQueueErrors RingQueue::getError() const;
std::string RingQueue::getErrorString(RingQueueErrors);
bool RingQueue::isOverrun() const {
    return false;
}
bool RingQueue::isUnderrun() const;
bool RingQueue::isEmpty() const;
bool RingQueue::isFull() const;
// these are for debugging
int RingQueue::nq() const;
int RingQueue::dq() const;
