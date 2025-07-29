#include "ConcurrencyStudies.h"

void accumulateRange(uint64_t &result, uint64_t start, uint64_t end) {

	result = 0;
	for (uint64_t i = start; i != end; i++) {
		result += i;
	}
	std::cout << "Thread " << std::this_thread::get_id()
			  << " over [" << std::setw(12) << start << ","
			  << std::setw(12) << end << ") stored "
			  << std::setw(24) << result << " to &result" << std::endl;
}

void sumUsingFunction(uint64_t number_of_elements) {

	for (int number_of_threads = 2; number_of_threads < 4; number_of_threads++) {
		std::cout << std::endl;
		std::cout << "* ***********************************	*" << std::endl;
		std::cout << "       number of threads: " << number_of_threads << std::endl;
		std::cout << "* ***********************************	*" << std::endl;
		std::cout << std::endl;


		uint64_t step = number_of_elements / number_of_threads;
		uint64_t step_remainder = number_of_elements % number_of_threads;

		std::vector<uint64_t> partial_sums(number_of_threads);
		std::vector<std::thread> threads(number_of_threads);

		for (int i = 0; i != number_of_threads; i++) {
			uint64_t start = i * step;
			uint64_t end = (i+1) * step;
			if (i == number_of_threads-1) {
				end += step_remainder;
			}
			std::cout << "launching accumulateRange(result, " << start
					  << " to " << end << ")" << std::endl;
			threads[i] = std::thread(accumulateRange, std::ref(partial_sums[i]), start, end);
		}

		for (std::thread &t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		uint64_t sum = std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));
		std::cout << "partial sums are: " << std:: endl << partialSumstoString<uint64_t>(partial_sums) << std::endl;
		std::cout << "Sum of integers from 0 to " << number_of_elements << " is " << sum << std::endl;
	}
}

