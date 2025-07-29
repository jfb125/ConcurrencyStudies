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
#include <stdint.h>
#include <vector>
#include <numeric>

#include "ConcurrencyStudies.h"

#define MAJOR_REVISION 1
#define MINOR_REVISION 0
#define BUILD_NUMBER 1
#define REVISION_STRING "1.00"

void sumUsingFunction(uint64_t number_of_elements);
void sumUsingFunctor (uint64_t number_of_elements);
void sumUsingLambdas (uint64_t number_of_elements);

template <typename T>
std::string partialSumstoString(std::vector<T> &partial_sums) {
	std::stringstream result;
	result << "{ " << std::endl;
	for (T i : partial_sums) {
		result << std::setw(24) << std::right << i << std::endl;
	}
	result << "}";
	return result.str();
}

#endif	/* #ifndef CONCURRENCY_STUDIES_H */
