/*
 * sumUsingAsync.cpp
 *
 *  Created on: Jul 31, 2025
 *      Author: joe
 */
#include "ConcurrencyStudies.h"

uint64_t asyncAccumulateFunction(uint64_t start, uint64_t end) {
	uint64_t result = 0;
	for (uint64_t i = start; i < end; i++) {
		result += i;
	}
//	std::cout << "thread " << std::this_thread::get_id() << ": " << std::setw(20) << start << " to " << std::setw(20) << end << " sums to " << std::setw(24) << result << std::endl;
	return result;
}

void sumUsingAsyncFunction (uint64_t number_of_elements, TestRange &test_cases) {

	std::cout 	<< std::endl
				<< "void sumUsingAsyncFunction()"
				<< std::endl;

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();
		uint64_t step_size = number_of_elements / number_of_threads;
		std::cout << "* ***********************************	*" << std::endl;
		std::cout << "       number of threads: " << number_of_threads << std::endl;

		uint64_t step_remainder = number_of_elements % number_of_threads;
		std::vector<std::future<uint64_t>> tasks;

		auto start_time = std::chrono::high_resolution_clock::now();

		uint64_t start = 0;
		uint64_t end = start+step_size;

		for (int i = 0; i != number_of_threads; i++) {
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
//			std::cout << " creating async(asyncAccumulateFunction(" << std::setw(20) << start << "," << std::setw(20) << end << ");" << std::endl;
			tasks.push_back(std::async(asyncAccumulateFunction, start, end));
			start = end;
			end += step_size;
		}

		uint64_t sum = 0;
		uint64_t partial_sums[number_of_threads];
		int i = 0;
		for (auto &t : tasks) {
			partial_sums[i] = t.get();
			sum += partial_sums[i];
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<uint64_t>(partial_sums, number_of_threads) << std::endl;
#endif
		std::cout << "\tSum of integers from 0 to " << number_of_elements << " is " << sum << std::endl;

		std::cout << "\tExecution time was: " << duration.count() << "ms" << std::endl;
	}

}


