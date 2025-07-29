/*
 * SumUsingFunctor.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */

#include "ConcurrencyStudies.h"

class AccumulateFunctor {
public:
	uint64_t m_sum;
	uint64_t m_start;
	uint64_t m_end;

	void operator() (uint64_t start, uint64_t end) {
		m_start = start;
		m_end = end;
		m_sum = 0;
		for (uint64_t i = start; i != end; i++) {
			m_sum += i;
		}
		std::cout << "thread #" << std::this_thread::get_id()
				  << " [" << std::setw(20) << start
				  << "," << std::setw(20) << end
				  << ") sum is " << std::setw(20) << m_sum << std::endl;
	}
};

void sumUsingFunctor (uint64_t number_of_elements) {

	int number_of_steps = 4;
	int step_size 		= number_of_elements / number_of_steps;
	int step_remainder	= number_of_elements % number_of_steps;
	std::vector<std::thread> threads;
	std::vector<class::AccumulateFunctor*> functors;

	for (int i = 0; i != number_of_steps; i++) {
		uint64_t start = i * step_size;
		uint64_t end = start + step_size;
		if (i == number_of_steps-1) {
			end += step_remainder;
		}
		std::cout << "launching thread [" << std::setw(16) << start << ", "
				  << std::setw(16) << end << ")" << std::endl;
		AccumulateFunctor *ftr = new AccumulateFunctor();
		threads.push_back(std::thread(std::ref(*ftr), start, end));
		functors.push_back(ftr);
	}

	for (std::thread &t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	uint64_t sum = 0;
	std::cout << "Partial sums are: " << std::endl << "{" << std::endl;
	for (auto &f : functors) {
		sum += f->m_sum;
		std::cout << std::setw(20) << f->m_sum << std::endl;
	}
	std::cout << "}" << std::endl;

	std::cout << "Sum is " << sum << std::endl;
}
