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

std::string threadNumberToString(int thread_number, int number_of_threads, int thread_number_width) {
	std::stringstream result;

	for (int i = 0; i != number_of_threads; i ++) {
		if (i == thread_number) {
			result << std::setw(thread_number_width) << i;
			while (++i < number_of_threads) {
				result << std::setw(thread_number_width) << " ";
			}
			break;
		}
		result << " ";
	}

	return result.str();
}


std::ostream& operator<<(std::ostream& out, Bark &object) {
	out << " produced by thread " << threadNumberToString(object.m_producer_thread_number, object.m_thread_map_width, 1)
		<< " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, object.m_thread_map_width, 1)
		<< " took " << std::setw(MAXIMUM_SLEEP_TIME_WIDTH) << object.m_consumer_sleep_time << " us, "
	    << "msg # " << std::setw(MESSAGE_NUMBER_WIDTH) << object.m_message_number << " \"" << *object.m_message << "\"";
	return out;
}



