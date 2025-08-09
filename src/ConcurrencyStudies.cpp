/*
 * ConcurrencyStudies.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <thread>
#include <stdint.h>
#include <vector>
#include <numeric>
#include <utility>
#include <cmath>
#include <format>

#include "ConcurrencyStudies.h"
#include "SimpleRandomizer.h"

constexpr const char * information =
//std::string information(
	"  This version of the program simulates multiple producer threads concurrently\n"
	"storing messages to a linear queue, which is then dequeued by consumer threads.\n"
    "The consumer queues wait a random time, then enqueue the messages into to a\n"
    "'retired' queue.  The retired queue is unloaded sequentially after threads terminate\n"
	;

void	testThreadsAndTasks();

constexpr int max_sleep_time_us = 100;

/* **************************************************************************** */
/* **************************************************************************** */
/* 							producer function									*/
/* **************************************************************************** */
/* **************************************************************************** */

/* **************************************************************************** */
/* **************************************************************************** */
/* 							consumer task										*/
/* **************************************************************************** */
/* **************************************************************************** */
#if 0
void consumeBark(std::atomic<int> &consumed_bark_count, int consumer_thread_number,
				 // variables that deal with the producer queue
				 std::shared_ptr<Bark> *produced_q,  std::atomic<int> &produced_enqueue, std::atomic<int> &produced_dequeue,
				 std::atomic<int> &active_thread_count, std::mutex &consumer_dequeue_lock,
				 // variables that deal with the consumer retired queue
				 std::shared_ptr<Bark> *retired_q, std::atomic<int> &retired_enqueue, std::mutex &consumer_retired_lock )  {
	std::shared_ptr<Bark> bark = nullptr;
	// while there are still active threads potentially enqueueing new messages
	while (active_thread_count != 0) {
		{	// begin scope of consumer_dequeue_lock guard
			std::lock_guard<std::mutex> q_lock(consumer_dequeue_lock);
			//	if there is a message / bark from a producer, acquire it
			if (produced_dequeue != produced_enqueue) {
				bark = produced_q[produced_dequeue];
				produced_q[produced_dequeue] = nullptr;
				produced_dequeue++;
			}
		}	// end scope of consumer_dqueue_lock guard
		if (bark) {
			//	simulate a varied processing time
			std::this_thread::sleep_for(std::chrono::microseconds(bark->m_consumer_sleep_time));
			//	retired the message / bark
			bark->m_consumer_thread_number = consumer_thread_number;
			{	// begin scope of consumer_retire_lock guard
				std::lock_guard<std::mutex> q_lock(consumer_retired_lock);
				retired_q[retired_enqueue] = bark;
				retired_enqueue++;
				consumed_bark_count++;
			}	// end scope of consumer_retired_lock guard
			bark = nullptr;
		}
	}
	// this only happens after active_producer_thread_count == 0
	//	which means no further messages are going to be enqueued.
	//	  all that happens from this point is that the remaining
	//	    messages get processed / retired
	while (true) {
		{	// begin scope of consumer_dequeue_lock guard
			std::lock_guard<std::mutex> q_lock(consumer_dequeue_lock);
			//	if not all of the produced msgs / barks have been dequeued
			if (produced_dequeue != produced_enqueue) {
				bark = produced_q[produced_dequeue];
				produced_q[produced_dequeue] = nullptr;
				produced_dequeue++;
			} else {
				// all of the enqueued have been dequeued - leave
				break;
			}
		}	// end scope of consumer_dequeue_lock guard
		// simulate a varied processing time
		std::this_thread::sleep_for(std::chrono::microseconds(bark->m_consumer_sleep_time));
		// retire the message
		bark->m_consumer_thread_number = consumer_thread_number;
		{	// begin scope of consumer_retire_lock guard
			std::lock_guard<std::mutex> q_lock(consumer_retired_lock);
			retired_q[retired_enqueue] = bark;
			retired_enqueue++;
			consumed_bark_count++;
		}	// end scope of consumer_retire_lock guard
		bark = nullptr;
	}
}
#endif
/* **************************************************************************** */
/* **************************************************************************** */
/* 							 		main function								*/
/* **************************************************************************** */
/* **************************************************************************** */

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl << std::endl;

	std::cout << information << std::endl;

	linearProducerConsumerTest();
//	testThreadsAndTasks();
	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}


/*  **************************************************************************************************  */
/*  **************************************************************************************************  */
/*          routines to help simple thread tests which only involve summing int ranges                  */
/*  **************************************************************************************************  */
/*  **************************************************************************************************  */

void announceResult(int number_of_threads, uint64_t sum, std::chrono::milliseconds duration) {
	std::cout << "    " << "number of threads: " << std::setw(3) << number_of_threads
			  << " result: " << std::setw(24) << sum
			  << " elapsed time: " << std::setw(5) << duration.count() << "ms"
			  << std::endl;
}

std::string partialSumsToString(uint64_t *partial_sums, int num_sums) {
    std::stringstream result;
    result << "{ " << std::endl;
    for (int i = 0; i != num_sums; i++) {
        result << std::setw(2) << i << ": " << std::setw(24) <<  partial_sums[i] << std::endl;
    }
    result << "}";
    return result.str();
}

void testThreadsAndTasks() {

	constexpr uint64_t number_of_elements = 1000 * 1000 * 1000;
	int _num_threads[] = { 1, 23, 211, 256 };
	int _num_num_threads = sizeof(_num_threads) / sizeof(int);
	TestRange number_of_threads(_num_threads, _num_num_threads);
	sumUsingFunction(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingFunctor(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingLambda(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncFunction(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncFunctor(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncLambda(number_of_elements, number_of_threads);
	std::cout << std::endl << std::endl;
}

