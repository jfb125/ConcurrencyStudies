/*
 * Producer.h
 *
 *  Created on: Aug 8, 2025
 *      Author: joe
 */

#ifndef LINEARPRODUCERCONSUMER_H_
#define LINEARPRODUCERCONSUMER_H_

#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <chrono>
#include <stdint.h>
#include <cmath>

#include "SimpleRandomizer.h"
#include "Bark.h"

/*  These implement a data flow where
 *   Producer threads
 *      enqueue messages ('barks') to a linear queue using a
 *      mutex to lock write access to the queue.  The
 *      number of Barks enqueued is passed as a parameter
 *      at the creation of the Producer thread
 *
 *   Consumer threads
 *      dequeue messages ('barks') from the linear queue using
 *      a mutex to lock read access to the queue
 *
 *   Simultaneous non-atomic access is allowed to & from the
 *      queue by having the Consumer compare the Producer's
 *      atomic variable 'enqueue' (index) to determine
 *      when 'enqueue' index is no longer equal (greater than)
 *      the Consumer's 'dequeue' index
 *
 *   Collisions between the consumer threads are promoted by
 *      simulating random processing times of the dequeued
 *      Barks from the Producer's queue, before writing
 *      the Bark to a retired queue. Access to the retired
 *      queue is managed by a retired mutex.
 *
 *   An atomic 'active_producer_thread' count is used by the
 *      Consumer threads to determine when all Producer threads
 *      have completed.  The Consumer threads then continue
 *      dequeueing Barks until the dequeue index == enqueue index
 *
 *   After all of the Consumer threads have processed Barks into
 *      the retired queue (retired enqueue == producer enqueue)
 *      a post processing loop runs that determines out-of-sequence
 *      ordering (which indicates potential contentions and
 *      prints out the contents of the retired queue
 */

#define LINEAR_PRODUCER_CONSUMER_MAX_SLEEP_TIME_US  100

class LinearProducerThreadArgs {
public:
    std::shared_ptr<std::string> id_msg;
    std::shared_ptr<std::string> exit_msg;
    int producer_thread_number; // this thread's number
    int repeat_count;           // how many times to execute the loop
    int *consumer_sleep_times;  // list of sleep times for the consumer
    std::shared_ptr<Bark> *produced_queue;
    LinearProducerThreadArgs(std::shared_ptr<std::string> _id_msg,
                       std::shared_ptr<std::string> _exit_msg,
                       int _thread_number,
                       int _repeat_count,
                       int*_consumer_sleep_times,
                       std::shared_ptr<Bark> *_produced_queue) :
                               id_msg(_id_msg),
                               exit_msg(_exit_msg),
                               producer_thread_number(_thread_number),
                               repeat_count(_repeat_count),
                               consumer_sleep_times(_consumer_sleep_times),
                               produced_queue(_produced_queue) {}
    LinearProducerThreadArgs() {
        id_msg                  = std::make_shared<std::string>(" ID  ");
        exit_msg                = std::make_shared<std::string>(" EXIT");
        producer_thread_number  = INVALID_THREAD_NUMBER;
        repeat_count            = 0;
        consumer_sleep_times    = nullptr;
        produced_queue          = nullptr;
    }
};

void produceBark(std::unique_ptr<LinearProducerThreadArgs> args, std::atomic<int> &enqueue, std::atomic<int> &active_thread_count, std::mutex &lock);
void consumeBark(std::atomic<int> &consumed_bark_count, int consumer_thread_number,
				 // variables that deal with the producer queue
				 std::shared_ptr<Bark> *produced_q,  std::atomic<int> &produced_enqueue, std::atomic<int> &produced_dequeue,
				 std::atomic<int> &active_thread_count, std::mutex &consumer_dequeue_lock,
				 // variables that deal with the consumer retired queue
				 std::shared_ptr<Bark> *retired_q, std::atomic<int> &retired_enqueue, std::mutex &consumer_retired_lock );
void testLinearProducerConsumer();

#endif /* LINEARPRODUCERCONSUMER_H_ */
