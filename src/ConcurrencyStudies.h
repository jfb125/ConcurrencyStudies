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
#include <memory>

#include "Bark.h"
#include "LinearProducerConsumer.h"

#define MAJOR_REVISION 3
#define MINOR_REVISION 1
#define BUILD_NUMBER 1
#define REVISION_STRING "3.01 Build 1"

#define MULTITHREADING_TEST_HEADER \
"* ******************************************************************************************** *\n"\
"* ******************************************************************************************** *\n"\
"* **** Testing performance improvement by splitting summing integers into multiple threads *** *\n"\
"* ******************************************************************************************** *\n"\
"* ******************************************************************************************** *\n"\
"\n"
#define MULTITHREADING_LATENCY_HEADER "LATENCIES (in milliseconds)"

#define MULTITHREADING_LINE_START_WIDTH 30
#define MULTITHREADING_LATENCY_WIDTH    12
//  a bunch of empirically determined magic numbers
#define printTestName(_s) do {\
    std::cout << std::setw(MULTITHREADING_LINE_START_WIDTH) << std::left << _s << std::right; \
} while(false)
#define printSumTestFailure(_result, _correct) do {\
	std::cout << "ERROR: result " << (_result) << " != expected " << (_correct) << std::endl;\
} while(false)


//  pass parameters to tests that sum up partial ranges in multiple threads
using sum_t = uint64_t;

class TestRange {
private:
	int *m_thread_sizes;
	int m_number_sizes;
	int m_current_size;
	sum_t m_correct_answer;

public:
	TestRange(int *_sizes, int _num, sum_t _correct_answer) :
	    m_thread_sizes(_sizes),
	    m_number_sizes(_num),
	    m_correct_answer(_correct_answer) {
	        m_current_size = 0;
	}
	void reset()	{ m_current_size = 0; }
	bool done()	{	return m_current_size == m_number_sizes; }
	int next() {
		int result = 0;	// will blow up the caller
		if (m_current_size < m_number_sizes) {
			result = m_thread_sizes[m_current_size++];
		}
		return result;
	}
	constexpr sum_t correct_answer() const {
	    return m_correct_answer;
	}
};

void mutexToLinearQueues();
void testThreadsAndTasks();

void sumUsingFunction(sum_t number_of_elements, TestRange &test_cases);
void sumUsingFunctor (sum_t number_of_elements, TestRange &test_cases);
void sumUsingLambda  (sum_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncFunction	 (sum_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncFunctor	 (sum_t number_of_elements, TestRange &test_cases);
void sumUsingAsyncLambda	 (sum_t number_of_elements, TestRange &test_cases);

void announceResult(int number_of_threads, sum_t sum, std::chrono::milliseconds duration);
std::string toCommaSeparatedString(sum_t number_of_elements);
std::string partialSumsToString(sum_t *partial_sums, int num_sums);

#endif	/* #ifndef CONCURRENCY_STUDIES_H */
