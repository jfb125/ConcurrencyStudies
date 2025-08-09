#include "ConcurrencyStudies.h"


void accumulateRange(sum_t &result, sum_t start, sum_t end) {

	result = 0;
	for (sum_t i = start; i != end; i++) {
		result += i;
	}
}

void sumUsingFunction(sum_t number_of_elements, TestRange &test_cases) {

    printTestName("void sumUsingFunction()");

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();

		sum_t step = number_of_elements / number_of_threads;
		sum_t step_remainder = number_of_elements % number_of_threads;

		sum_t partial_sums[number_of_threads];
		std::vector<std::thread> threads(number_of_threads);

		auto start_time = std::chrono::high_resolution_clock::now();
		for (int i = 0; i != number_of_threads; i++) {
			sum_t start = i * step;
			sum_t end = (i+1) * step;
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
			threads[i] = std::thread(accumulateRange, std::ref(partial_sums[i]), start, end);
		}

		for (std::thread &t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		sum_t sum = 0;
		for (int i = 0; i != number_of_threads; i++) {
			sum += partial_sums[i];
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<sum_t>(partial_sums, number_of_threads) << std::endl;
#endif
		announceResult(number_of_threads, sum, duration);

		if (sum != test_cases.correct_answer()) {
		    printSumTestFailure(sum, test_cases.correct_answer());
		    return;
		}
	}
	std::cout << std::endl;
}

