/*
 * Bark.cpp
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#include "Bark.h"

//	prints out a single digit thread number in a field "   3    " or "0      " or "       7"
//	which makes looking at a dump of the queues a little more intuitive:
//	   3       " some message "
//	0          " some other message "
//         7   " some other other message "

std::string threadNumberToString(int thread_number, int thread_number_width, int number_of_threads) {
	std::stringstream result;
	if (thread_number >= number_of_threads) {
		result << thread_number;
	} else {
		for (int i = 0; i != thread_number; i++) {
			result << std::setw(thread_number_width) << " ";
		}
		result << std::setw(thread_number_width) << thread_number;
		for (int i = thread_number+1; i != number_of_threads; i++) {
			result << std::setw(thread_number_width) << " ";
		}
	}

	return result.str();
}

#define MAXIMUM_SLEEP_TIME_WIDTH 3
#define MESSAGE_NUMBER_WIDTH 3

std::ostream& operator<<(std::ostream& out, Bark &object) {
	out << " produced by thread " << threadNumberToString(object.m_producer_thread_number, object.m_thread_map_width, 1)
		<< " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, object.m_thread_map_width, 1)
		<< " took " << std::setw(MAXIMUM_SLEEP_TIME_WIDTH) << object.m_consumer_sleep_time << " us, "
	    << "msg # " << std::setw(MESSAGE_NUMBER_WIDTH) << object.m_message_number << " \"" << *object.m_message << "\"";
	return out;
}
#undef MAXIMUM_SLEEP_TIME_WIDTH
#undef MESSAGE_NUMBER_WIDTH

std::string toString(Bark &object,
					 int producer_thread_number_width, int producer_number_of_threads,
					 int consumer_thread_number_width, int consumer_number_of_threads,
					 int sleep_time_width, int message_number_width) {
	std::stringstream ret;
	ret << " produced by thread " << threadNumberToString(object.m_producer_thread_number, object.m_thread_map_width, 1)
		<< " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, object.m_thread_map_width, 1)
		<< " took " << std::setw(sleep_time_width) << object.m_consumer_sleep_time << " us, "
	    << "msg # " << std::setw(message_number_width) << object.m_message_number << " \"" << *object.m_message << "\"";
	return ret.str();
}


