#include "ConcurrencyStudies.h"


void accumulateRange(uint64_t &result, uint64_t start, uint64_t end) {

	result = 0;
	for (uint64_t i = start; i != end; i++) {
		result += i;
	}
	if (false) {
		std::cout << "Thread " << std::this_thread::get_id()
				  << " over [" << std::setw(12) << start << ","
				  << std::setw(12) << end << ") stored "
				  << std::setw(24) << result << " to &result" << std::endl;
	}
}

void sumUsingFunction(uint64_t number_of_elements, TestRange &test_cases) {

	std::cout 	<< std::endl
				<< "void sumUsingFunction()"
				<< std::endl;

	test_cases.reset();
	while (!test_cases.done()) {
		int number_of_threads = test_cases.next();

		uint64_t step = number_of_elements / number_of_threads;
		uint64_t step_remainder = number_of_elements % number_of_threads;

		uint64_t partial_sums[number_of_threads];
		std::vector<std::thread> threads(number_of_threads);

		auto start_time = std::chrono::high_resolution_clock::now();
		for (int i = 0; i != number_of_threads; i++) {
			uint64_t start = i * step;
			uint64_t end = (i+1) * step;
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

		uint64_t sum = 0;
		for (int i = 0; i != number_of_threads; i++) {
			sum += partial_sums[i];
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

#ifdef PRINT_PARTIAL_SUMS
		std::cout << "partial sums are: " << std:: endl << partialSumsToString<uint64_t>(partial_sums, number_of_threads) << std::endl;
#endif
		announceResult(number_of_threads, sum, duration);
	}
}

