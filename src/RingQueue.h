/*
 * RingQueue.h
 *
 *  Created on: Aug 8, 2025
 *      Author: joe
 */

#ifndef RINGQUEUE_H_
#define RINGQUEUE_H_

#include <iostream>
#include <string>
#include <iomanip>
#include <atomic>
#include <memory>
#include <mutex>
#include <future>

template <typename T>
class RingQueue {
   int  m_size;
   std::atomic<int>     m_nq;
   std::atomic<int>     m_dq;
   std::atomic<bool>    m_full;     // no room for more elements
   std::atomic<bool>    m_empty;    // no elements
   std::atomic<bool>    m_overrun;  // an attempt to enqueue was made while full
   std::atomic<bool>    m_underrun; // an attempt to dequeue was mmde while empty
   std::unique_ptr<T*>  m_queue;    // storage

public:
   enum class RingQueueErrors {
       OVER_RUN_ERROR           = 1,
       UNDER_RUN_ERROR          = 2,
       OVER_RUN_AND_UNDER_RUN   = 3,
       NO_ERROR                 = 0
   };

   RingQueue() = delete;
   RingQueue(int size);
   virtual ~RingQueue();
   RingQueue(const RingQueue &other);
   RingQueue(RingQueue &&other);
   RingQueue& operator=(const RingQueue &other);
   RingQueue& operator=(RingQueue &&other);

   void reset();
   bool enqueue(T* &p_object);
   T*   dequeue();
   bool isError() const;
   RingQueueErrors getError() const;
   std::string getErrorString(RingQueueErrors);
   bool isOverrun() const;
   bool isUnderrun() const;
   bool isEmpty() const;
   bool isFull() const;
   // these are for debugging
   int nq() const;
   int dq() const;
};

#endif /* RINGQUEUE_H_ */
