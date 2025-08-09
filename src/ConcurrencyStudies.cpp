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

/* **************************************************************************** */
/* **************************************************************************** */
/* 							 		main function								*/
/* **************************************************************************** */
/* **************************************************************************** */

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl << std::endl;

	testThreadsAndTasks();
	testLinearProducerConsumer();

	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}


/*  **************************************************************************************************  */
/*  **************************************************************************************************  */
/*          routines to help simple thread tests which only involve summing int ranges                  */
/*  **************************************************************************************************  */
/*  **************************************************************************************************  */

void testThreadsAndTasks() {

    constexpr sum_t number_of_elements = 1000 * 1000 * 1000;
    int _num_threads[] = { 1, 2, 64, 128, 150, 200 };
    int _num_num_threads = sizeof(_num_threads) / sizeof(int);

    std::cout << MULTITHREADING_TEST_HEADER << std::endl;
    std::cout << "Summing from 0 to " << toCommaSeparatedString(number_of_elements) << " ";
    // setw() using magic number ...
    std::cout << std::setw(MULTITHREADING_LINE_START_WIDTH+MULTITHREADING_LATENCY_WIDTH) << MULTITHREADING_LATENCY_HEADER << std::endl;
    std::cout << std::setw(MULTITHREADING_LINE_START_WIDTH) << std::left << "Number of threads" << std::right;
    for (int i = 0; i != _num_num_threads; i++) {
        std::cout << std::setw(MULTITHREADING_LATENCY_WIDTH) << _num_threads[i];
    }
    std::cout << std::endl;
    std::cout << "------------------------------" << std::endl;

    sum_t correct_answer = 0;
    for (sum_t i = 0; i != number_of_elements; i++) {
        correct_answer += i;
    }
    TestRange number_of_threads(_num_threads, _num_num_threads, correct_answer);

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


/*  **************************************************************************************************  */
/*  **************************************************************************************************  */
/*          routines to help simple thread tests which only involve summing int ranges                  */
/*  **************************************************************************************************  */
/*  **************************************************************************************************  */

void announceResult(int number_of_threads, sum_t sum, std::chrono::milliseconds duration) {
#if 0
    std::cout << "    " << "number of threads: " << std::setw(3) << number_of_threads
			  << " result: " << std::setw(24) << sum
			  << " elapsed time: " << std::setw(5) << duration.count() << "ms"
			  << std::endl;
#endif
    std::cout << std::setw(MULTITHREADING_LATENCY_WIDTH) << duration.count();
}

std::string partialSumsToString(sum_t *partial_sums, int num_sums) {
    std::stringstream result;
    result << "{ " << std::endl;
    for (int i = 0; i != num_sums; i++) {
        result << std::setw(2) << i << ": " << std::setw(24) <<  partial_sums[i] << std::endl;
    }
    result << "}";
    return result.str();
}

std::string toCommaSeparatedString(sum_t number_of_elements) {

    std::stringstream result;

    // don't pass a number < 1000 to the loops, which rely on their
    //  being at least two groups
    if (number_of_elements < 1000) {
        result << number_of_elements;
        return result.str();
    }

    int num_groups = static_cast<int>(std::log10(number_of_elements)/3)+1;
    int groups[num_groups];

    for (int group_num = num_groups-1; group_num >= 0; --group_num) {
        groups[group_num] = number_of_elements % 1000;
        number_of_elements /= 1000;
    }

    // most significant group does not have leading 0s
    result << groups[0] << ',';
    //  middle groups have leading zeroes and a ','
    for (int i = 1; i != num_groups-1; i++) {
        result << std::setw(3) << std::setfill('0') << groups[i] << ',';
    }
    //  least significant group does not have a trailing ','
    result << std::setw(3) << std::setfill('0') << groups[num_groups-1];

    return result.str();
}

