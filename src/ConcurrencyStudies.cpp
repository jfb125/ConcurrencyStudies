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

#include "ConcurrencyStudies.h"

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl;

	constexpr uint64_t number_of_elements = 1000 * 1000 * 1000;
	int _num_threads[] = { 7, 19, 211 };
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
	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}

void announceResult(int number_of_threads, uint64_t sum, std::chrono::milliseconds duration) {
	std::cout << "\t" << "number of threads: " << std::setw(3) << number_of_threads
			  << " result: " << std::setw(24) << sum
			  << " elapsed time: " << std::setw(5) << duration.count() << "ms"
			  << std::endl;
}


