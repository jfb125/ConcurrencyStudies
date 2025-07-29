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

#include "ConcurrencyStudies.h"

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl;

	constexpr uint64_t number_of_elements = 1000 * 1000 * 1000;
//	sumUsingFunction(number_of_elements);
	sumUsingFunctor(number_of_elements);
	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}



