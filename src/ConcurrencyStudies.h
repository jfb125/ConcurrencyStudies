/*
 * ConcurrencyStudies.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */
#ifndef CONCURRENCYSTUDIES_H
#define CONCURRENCYSTUDIES_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <future>
#include <stdint.h>
#include <vector>
#include <numeric>
#include <chrono>

#include "Bark.h"

#define MAJOR_REVISION 3
#define MINOR_REVISION 1
#define BUILD_NUMBER 1
#define REVISION_STRING "3.01 Build 1"

//#define PRINT_PARTIAL_SUMS

class TestRange {
private:
	int *m_thread_sizes;
	int m_number_sizes;
	int current_size;
public:
	TestRange(int *_sizes, int _num) : m_thread_sizes(_sizes), m_number_sizes(_num) { current_size = 0; }
	void reset()	{ current_size = 0; }
	bool done()	{	return current_size == m_number_sizes; }
	int next() {
		int result = 0;	// will blow up the caller
		if (current_size < m_number_sizes) {
			result = m_thread_sizes[current_size++];
		}
		return result;
	}
};

void sumUsingFunction(uint64_t number_of_elements, TestRange &test_cases);
void sumUsingFunctor (uint64_t number_of_elements, TestRange &test_cases);
void sumUsingLambda  (uint64_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncFunction	 (uint64_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncFunctor	 (uint64_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncLambda	 (uint64_t number_of_elements, TestRange &test_cases);

void announceResult(int number_of_threads, uint64_t sum, std::chrono::milliseconds duration);

template <typename T>
std::string partialSumsToString(uint64_t *partial_sums, int num_sums) {
	std::stringstream result;
	result << "{ " << std::endl;
	for (int i = 0; i != num_sums; i++) {
		result << std::setw(2) << i << ": " << std::setw(24) <<  partial_sums[i] << std::endl;
	}
	result << "}";
	return result.str();
}

#endif	/* #ifndef CONCURRENCY_STUDIES_H */
