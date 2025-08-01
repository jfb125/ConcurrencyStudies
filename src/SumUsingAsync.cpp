/*
 * sumUsingAsync.cpp
 *
 *  Created on: Jul 31, 2025
 *      Author: joe
 */
#include "ConcurrencyStudies.h"

/*	**********************************************	*/
/*	**********************************************	*/
/*				function ptr						*/
/*	**********************************************	*/
/*	**********************************************	*/

uint64_t asyncAccumulateFunction(uint64_t start, uint64_t end) {
	uint64_t result = 0;
	for (uint64_t i = start; i < end; i++) {
		result += i;
	}
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
		uint64_t step_remainder = number_of_elements % number_of_threads;
		std::vector<std::future<uint64_t>> tasks;

		auto start_time = std::chrono::high_resolution_clock::now();

		uint64_t start = 0;
		uint64_t end = start+step_size;

		for (int i = 0; i != number_of_threads; i++) {
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
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
		announceResult(number_of_threads, sum, duration);
	}

}

/*	**********************************************	*/
/*	**********************************************	*/
/*				functor object						*/
/*	**********************************************	*/
/*	**********************************************	*/

class AsyncFunctor {
public:
	uint64_t result;
	AsyncFunctor() : result(0) {}
	uint64_t operator()(uint64_t start, uint64_t end) {
		result = 0;
		for (uint64_t i = start; i < end; i++) {
			result += i;
		}
		return result;
	}
};

void sumUsingAsyncFunctor	 (uint64_t number_of_elements, TestRange &test_cases) {
	std::cout 	<< std::endl
				<< "void sumUsingAsyncFunctor()"
				<< std::endl;

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_tasks = test_cases.next();
		uint64_t step_size = number_of_elements / number_of_tasks;
		uint64_t step_remainder = number_of_elements % number_of_tasks;
		std::vector<std::future<uint64_t>> tasks;
		std::vector<AsyncFunctor*> functors;

		auto start_time = std::chrono::high_resolution_clock::now();

		uint64_t start = 0;
		uint64_t end = start+step_size;

		AsyncFunctor *object;

		for (int i = 0; i != number_of_tasks; i++) {
			if (i == number_of_tasks-1) {
				end += step_remainder;
			}
			object = new AsyncFunctor();
			functors.push_back(object);
			tasks.push_back(std::async(std::ref(*object), start, end));
			start = end;
			end += step_size;
		}

		uint64_t sum = 0;
		for (auto &t : tasks) {
			sum += t.get();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	#ifdef PRINT_PARTIAL_SUMS
		uint64_t partial_sums[number_of_tasks];
		for (int i = 0; i != number_of_tasks; i++) {
			partial_sums[i] = functors[i]->result;
		}
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<uint64_t>(partial_sums, number_of_tasks) << std::endl;
	#endif
		announceResult(number_of_tasks, sum, duration);
	}
}

/*	**********************************************	*/
/*	**********************************************	*/
/*					lambda							*/
/*	**********************************************	*/
/*	**********************************************	*/

void sumUsingAsyncLambda	 (uint64_t number_of_elements, TestRange &test_cases) {
	std::cout 	<< std::endl
				<< "void sumUsingLambdas()"
				<< std::endl;

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_tasks = test_cases.next();
		uint64_t step_size = number_of_elements / number_of_tasks;
		uint64_t step_remainder = number_of_elements % number_of_tasks;
		uint64_t start = 0;
		uint64_t end = step_size;
		uint64_t partial_sums[number_of_tasks];
		std::vector<std::future<uint64_t>> tasks;

		auto start_time = std::chrono::high_resolution_clock::now();

		for (int i = 0; i != number_of_tasks; i++) {
			if (i == number_of_tasks - 1) {
				end = end+step_remainder;
			}
			tasks.push_back(std::async([i, start, end] () -> uint64_t {
				uint64_t result = 0;
				for (uint64_t x = start; x < end; x++) {
					result += x;
				}
				return result;
			}
			));
			start = end;
			end = end + step_size;
		}

		uint64_t sum = 0;
		uint64_t *ps = partial_sums;
		for (auto &t : tasks) {
			*ps = t.get();
			sum += *ps++;
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "Partial sums are " << partialSumsToString<uint64_t>(partial_sums, number_of_tasks) << std::endl;
#endif
		announceResult(number_of_tasks, sum, duration);
	}
}
