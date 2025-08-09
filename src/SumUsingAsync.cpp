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

sum_t asyncAccumulateFunction(sum_t start, sum_t end) {
	sum_t result = 0;
	for (sum_t i = start; i < end; i++) {
		result += i;
	}
	return result;
}

void sumUsingAsyncFunction (sum_t number_of_elements, TestRange &test_cases) {

    printTestName("void sumUsingAsyncFunction()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();
		sum_t step_size = number_of_elements / number_of_threads;
		sum_t step_remainder = number_of_elements % number_of_threads;
		std::vector<std::future<sum_t>> tasks;

		auto start_time = std::chrono::high_resolution_clock::now();

		sum_t start = 0;
		sum_t end = start+step_size;

		for (int i = 0; i != number_of_threads; i++) {
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
			tasks.push_back(std::async(asyncAccumulateFunction, start, end));
			start = end;
			end += step_size;
		}

		sum_t sum = 0;
		sum_t partial_sums[number_of_threads];
		int i = 0;
		for (auto &t : tasks) {
			partial_sums[i] = t.get();
			sum += partial_sums[i];
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<sum_t>(partial_sums, number_of_threads) << std::endl;
#endif
		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, (test_cases.correct_answer()));
		}
		announceResult(number_of_threads, sum, duration);
	}
	std::cout << std::endl;
}

/*	**********************************************	*/
/*	**********************************************	*/
/*				functor object						*/
/*	**********************************************	*/
/*	**********************************************	*/

class AsyncFunctor {
public:
	sum_t result;
	AsyncFunctor() : result(0) {}
	sum_t operator()(sum_t start, sum_t end) {
		result = 0;
		for (sum_t i = start; i < end; i++) {
			result += i;
		}
		return result;
	}
};

void sumUsingAsyncFunctor	 (sum_t number_of_elements, TestRange &test_cases) {

    printTestName("void sumUsingAsyncFunctor()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_tasks = test_cases.next();
		sum_t step_size = number_of_elements / number_of_tasks;
		sum_t step_remainder = number_of_elements % number_of_tasks;
		std::vector<std::future<sum_t>> tasks;
		std::vector<AsyncFunctor*> functors;

		auto start_time = std::chrono::high_resolution_clock::now();

		sum_t start = 0;
		sum_t end = start+step_size;

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

		sum_t sum = 0;
		for (auto &t : tasks) {
			sum += t.get();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	#ifdef PRINT_PARTIAL_SUMS
		sum_t partial_sums[number_of_tasks];
		for (int i = 0; i != number_of_tasks; i++) {
			partial_sums[i] = functors[i]->result;
		}
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<sum_t>(partial_sums, number_of_tasks) << std::endl;
	#endif
		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, (test_cases.correct_answer()));
		    return;
		}
		announceResult(number_of_tasks, sum, duration);
	}
	std::cout << std::endl;
}

/*	**********************************************	*/
/*	**********************************************	*/
/*					lambda							*/
/*	**********************************************	*/
/*	**********************************************	*/

void sumUsingAsyncLambda	 (sum_t number_of_elements, TestRange &test_cases) {
    printTestName("void sumUsingAsyncLambdas()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_tasks = test_cases.next();
		sum_t step_size = number_of_elements / number_of_tasks;
		sum_t step_remainder = number_of_elements % number_of_tasks;
		sum_t start = 0;
		sum_t end = step_size;
		sum_t partial_sums[number_of_tasks];
		std::vector<std::future<sum_t>> tasks;

		auto start_time = std::chrono::high_resolution_clock::now();

		for (int i = 0; i != number_of_tasks; i++) {
			if (i == number_of_tasks - 1) {
				end = end+step_remainder;
			}
			tasks.push_back(std::async([i, start, end] () -> sum_t {
				sum_t result = 0;
				for (sum_t x = start; x < end; x++) {
					result += x;
				}
				return result;
			}
			));
			start = end;
			end = end + step_size;
		}

		sum_t sum = 0;
		sum_t *ps = partial_sums;
		for (auto &t : tasks) {
			*ps = t.get();
			sum += *ps++;
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "Partial sums are " << partialSumsToString<sum_t>(partial_sums, number_of_tasks) << std::endl;
#endif
		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, (test_cases.correct_answer()));
		}
		announceResult(number_of_tasks, sum, duration);
	}
	std::cout << std::endl;
}
