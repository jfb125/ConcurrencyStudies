/*
 * SumUsingFunctor.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */

#include "ConcurrencyStudies.h"

class AccumulateFunctor {
public:
	sum_t m_sum;
	sum_t m_start;
	sum_t m_end;

	void operator() (sum_t start, sum_t end) {
		m_start = start;
		m_end = end;
		m_sum = 0;
		for (sum_t i = start; i != end; i++) {
			m_sum += i;
		}
	}
};

void sumUsingFunctor (sum_t number_of_elements, TestRange &test_cases) {

    printTestName("void sumUsingFunctor()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();
		int step_size 		= number_of_elements / number_of_threads;
		int step_remainder	= number_of_elements % number_of_threads;
		std::vector<std::thread> threads;
		std::vector<class::AccumulateFunctor*> functors;

		auto start_time = std::chrono::high_resolution_clock::now();
		for (int i = 0; i != number_of_threads; i++) {
			sum_t start = i * step_size;
			sum_t end = start + step_size;
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
			AccumulateFunctor *ftr = new AccumulateFunctor();
			threads.push_back(std::thread(std::ref(*ftr), start, end));
			functors.push_back(ftr);
		}

		for (std::thread &t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		sum_t sum = 0;
		for (auto &f : functors) {
			sum += f->m_sum;
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		sum_t partial_sums[number_of_threads];
		for (int i = 0; i != number_of_threads; i++) {
			partial_sums[i] = functors[i]->m_sum;
		}
		std::cout << "Partial sums are: " << partialSumsToString<sum_t>(partial_sums, number_of_threads) << std::endl;
#endif
		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, test_cases.correct_answer());
		    return;
		}
	    announceResult(number_of_threads, sum, duration);
	}
	std::cout << std::endl;
}
