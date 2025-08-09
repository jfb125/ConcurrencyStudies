/*
 * UsingLambdas.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */

#include "ConcurrencyStudies.h"

void sumUsingLambda(sum_t number_of_elements, TestRange &test_cases) {

    printTestName("void sumUsingLambdas()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();
		sum_t step_size = number_of_elements / number_of_threads;
		sum_t step_remainder = number_of_elements % number_of_threads;
		sum_t start = 0;
		sum_t end = step_size;
		sum_t partial_sums[number_of_threads];
		std::vector<std::thread> threads;

		auto start_time = std::chrono::high_resolution_clock::now();

		for (int i = 0; i != number_of_threads; i++) {
			if (i == number_of_threads - 1) {
				end = end+step_remainder;
			}
			threads.push_back(std::thread([i, &partial_sums, start, end] () {
				partial_sums[i] = 0;
				for (sum_t x = start; x < end; x++) {
					partial_sums[i] += x;
				}
			}
			));
			start = end;
			end = end + step_size;
		}

		for (auto &t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		sum_t sum = 0;
		for (int i = 0; i != number_of_threads; i++) {
			sum += partial_sums[i];
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "Partial sums are " << partialSumsToString<sum_t>(partial_sums, number_of_threads) << std::endl;
#endif
		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, (test_cases.correct_answer()));
		    return;
		}
		announceResult(number_of_threads, sum, duration);
	}
	std::cout << std::endl;
}
