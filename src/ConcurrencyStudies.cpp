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

#include "ConcurrencyStudies.h"
#include "SimpleRandomizer.h"

std::string information(
	"  This version of the program simulates multiple producer threads concurrently\n"
	"storing messages to a queue, which is then dequeued by multiple consumer threads.\n"
	"Each message has a variable delay time that the consumer should wait before \n"
	"retiring a message to a 'retired' queue\n\n"
	);

void	testThreadsAndTasks();

onstexpr unsigned max_sleep_time_us	= 100;


/* **************************************************************************** */
/* **************************************************************************** */
/* 							producer function									*/
/* **************************************************************************** */
/* **************************************************************************** */

class ProducerThreadArgs {
public:
	ProducerThreadArgs() {
		id_msg 		= std::make_shared<std::string>(" ID  ");
		exit_msg 	= std::make_shared<std::string>(" EXIT");
		producer_thread_number = INVALID_THREAD_NUMBER;
		thread_map_width = 8;
		repeat_count = 0;
		consumer_sleep_times = nullptr;
		produced_queue = nullptr;
	}
public:
	std::shared_ptr<std::string> id_msg;
	std::shared_ptr<std::string> exit_msg;
	int producer_thread_number;	// this thread's number
	int thread_map_width;		// used for formatted output
	int repeat_count;			// how many times to execute the loop
	int *consumer_sleep_times;	// list of sleep times for the consumer
	std::shared_ptr<Bark> *produced_queue;
	ProducerThreadArgs(std::shared_ptr<std::string> _id_msg,
					   std::shared_ptr<std::string> _exit_msg,
					   int _thread_number,
					   int _thread_map_width,
					   int _repeat_count,
					   int*_consumer_sleep_times,
					   std::shared_ptr<Bark> *_produced_queue) :
							   id_msg(_id_msg),
							   exit_msg(_exit_msg),
							   producer_thread_number(_thread_number),
							   thread_map_width(_thread_map_width),
							   repeat_count(_repeat_count),
							   consumer_sleep_times(_consumer_sleep_times),
							   produced_queue(_produced_queue) {}
};

void produceBark(std::unique_ptr<ProducerThreadArgs> args, std::atomic<int> &enqueue, std::atomic<int> &active_thread_count, std::mutex &lock) {

	std::shared_ptr<Bark>tmp;
	// generate the specified number of messages / barks
	for (int i = 0; i != args->repeat_count; i++) {
		tmp = std::make_shared<Bark>(args->thread_map_width, args->producer_thread_number, i, args->consumer_sleep_times[i], args->id_msg);
		{
			std::lock_guard<std::mutex> q_lock(lock);
			args->produced_queue[enqueue] = tmp;
			enqueue++;
		}
	}
	// generate the final message
	tmp = std::make_shared<Bark>(args->thread_map_width, args->producer_thread_number, args->repeat_count, args->consumer_sleep_times[args->repeat_count], args->exit_msg);
	{
		std::lock_guard<std::mutex> q_lock(lock);
		args->produced_queue[enqueue] = tmp;
		enqueue++;
		active_thread_count--;
	}
}


/* **************************************************************************** */
/* **************************************************************************** */
/* 							consumer task										*/
/* **************************************************************************** */
/* **************************************************************************** */

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


/* **************************************************************************** */
/* **************************************************************************** */
/* 							 		main function								*/
/* **************************************************************************** */
/* **************************************************************************** */

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl << std::endl;

	std::cout << information << std::endl;

	SimpleRandomizer randomizer(getChronoSeed());

	int number_of_tests = 8;

	double average_collisions = 0.0;
	int tests_so_far = 0;
	int average_width = 0;
	bool print_out_queue = true;

	for (int test_count = 0; test_count != number_of_tests; test_count++) {
		std::mutex producer_enqueue_lock;
		std::mutex consumer_dequeue_lock;
		std::mutex consumer_retired_lock;
		int number_of_producer_threads = 8;
		int number_of_consumer_threads = 8;
		int thread_map_width = number_of_producer_threads;	// for formatted output
		uint64_t repeat_per_thread = 100;
		// number of times a string will be enqueued per thread * number_of_threads +
		//	one more for the "exiting thread" message from each queue
		int producer_queue_size = repeat_per_thread * number_of_producer_threads + number_of_producer_threads;
		int consumer_queue_size = producer_queue_size;
		std::shared_ptr<Bark> 	*produced_queue = new std::shared_ptr<Bark>[producer_queue_size];
		std::shared_ptr<Bark> 	*retired_queue  = new std::shared_ptr<Bark>[consumer_queue_size];
		// determine the number of digits in the average for later formatted output
		if (average_width == 0) {
			average_width = 1 + static_cast<int>(std::log10(static_cast<double>(producer_queue_size)));
		}
		std::atomic<int> produced_enqueue = 0;
		std::atomic<int> produced_dequeue = 0;
		std::atomic<int> retired_enqueue = 0;
		std::atomic<int> active_producer_thread_count = number_of_producer_threads;
		std::vector<std::thread> producer_threads;
		std::vector<std::thread> consumer_threads;

		std::atomic<int> consumed_bark_count = 0;
		std::shared_ptr<std::string> id_msg 	= std::make_shared<std::string>("    barks   ");
		std::shared_ptr<std::string> exit_msg 	= std::make_shared<std::string>(" ***EXITS***");

		//	create the consumer sleep times pseudo randomly, which the threads will then
		//	  insert into the 'sleep_time' member of the bark.  The consumer thread will
		//	  wait for this 'sleep_time' in us before retiring a thread, simulating a
		//	  random processing time for messages of different complexity
		int* consumer_sleep_times[number_of_producer_threads];
		for (int i = 0; i != number_of_producer_threads; i++) {
			consumer_sleep_times[i] = new int[repeat_per_thread+1];
			for (uint64_t j = 0; j != repeat_per_thread+1; j++) {
				consumer_sleep_times[i][j] = static_cast<int>(randomizer.rand(static_cast<ConcurrentRandomNumber>(1), static_cast<ConcurrentRandomNumber>(max_sleep_time_us)));
			}
		}

		//	launch the consumer threads first so that the producer threads will not flood the producer queue
		for (int consumer_thread_number = 0; consumer_thread_number != number_of_consumer_threads; consumer_thread_number++) {
			consumer_threads.push_back(std::thread(consumeBark,
													std::ref(consumed_bark_count), consumer_thread_number,
													produced_queue, std::ref(produced_enqueue), std::ref(produced_dequeue),
													std::ref(active_producer_thread_count), std::ref(consumer_dequeue_lock),
													retired_queue, std::ref(retired_enqueue), std::ref(consumer_retired_lock)));
		}

		for (int producer_thread_number = 0; producer_thread_number != number_of_producer_threads; producer_thread_number++) {
			std::unique_ptr<ProducerThreadArgs> producer_args =
					std::make_unique<ProducerThreadArgs>(
							id_msg, exit_msg,
							producer_thread_number,
							thread_map_width,
							repeat_per_thread,
							consumer_sleep_times[producer_thread_number],
							produced_queue
							);
			producer_threads.push_back(std::thread(produceBark, std::move(producer_args), std::ref(produced_enqueue), std::ref(active_producer_thread_count), std::ref(producer_enqueue_lock)));
		}

		for (std::thread &t : producer_threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		for (std::thread &t : consumer_threads) {
			if (t.joinable()){
				t.join();
			}
		}

		if (print_out_queue) {
			std::cout << "enqueued strings: " << produced_enqueue << " vs consumer & retired strings " << retired_enqueue << std::endl;
			std::cout << consumed_bark_count << " strings were consumed in this order: " << std::endl;
		}

		int out_of_order_count = 0;
		if (print_out_queue)
			std::cout << std::setw(4) << "0" << ": " << *retired_queue[0] << std::endl;
		for (int retired_dequeue = 1; retired_dequeue != consumed_bark_count; retired_dequeue++) {
			if (print_out_queue)
				std::cout << std::setw(4) << retired_dequeue << ": " << *retired_queue[retired_dequeue];
			if ((retired_queue[retired_dequeue])->m_producer_thread_number != (retired_queue[retired_dequeue-1])->m_producer_thread_number) {
				out_of_order_count++;
				if (print_out_queue)
					std::cout << " ++++ INTERLEAVED ++++";
			}
			if (print_out_queue) {
				std::cout << std::endl;
			}
		}

		std::cout << "test #" << std::setw(average_width) << test_count << ": out of order count: " << out_of_order_count << std::endl;

		tests_so_far++;
		average_collisions = average_collisions + (static_cast<double>(out_of_order_count) - average_collisions)/tests_so_far;
		delete[] produced_queue;
		delete[] retired_queue;
		for (int i = 0; i != number_of_producer_threads; i++) {
			delete[] consumer_sleep_times[i];
			consumer_sleep_times[i] = nullptr;
		}
	}
	std::cout << "average number of collisions: "  << std::setw(average_width) << std::fixed << std::setprecision(0) << average_collisions << std::endl;

	//	testThreadsAndTasks();
	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}

void announceResult(int number_of_threads, uint64_t sum, std::chrono::milliseconds duration) {
	std::cout << "\t" << "number of threads: " << std::setw(3) << number_of_threads
			  << " result: " << std::setw(24) << sum
			  << " elapsed time: " << std::setw(5) << duration.count() << "ms"
			  << std::endl;
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
