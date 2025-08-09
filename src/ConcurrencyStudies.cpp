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


/* **************************************************************************** */
/* **************************************************************************** */
/* 							 		main function								*/
/* **************************************************************************** */
/* **************************************************************************** */

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl << std::endl;

	std::cout << information << std::endl;

<<<<<<< HEAD
	linearProducerConsumerTest();
//	testThreadsAndTasks();
=======
	testThreadsAndTasks();
	testLinearProducerConsumer();

>>>>>>> RingQueue
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

