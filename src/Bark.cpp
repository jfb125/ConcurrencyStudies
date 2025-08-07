/*
 * Bark.cpp
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#include "Bark.h"
#include <fmt/format.h>

//	prints out a single digit thread number in a field "   3    " or "0      " or "       7"
//	which makes looking at a dump of the queues a little more intuitive:
//	   3       " some message "
//	0          " some other message "
//         7   " some other other message "

std::string threadNumberToString(int thread_number, int thread_map_width) {
	std::stringstream result;
	for (int i = 0; i != thread_map_width; i ++) {
		if (i == thread_number) {
			result << i;
			while (++i < thread_map_width) {
				result << " ";
			}
			break;
		}
		result << " ";
	}

	return result.str();
}


std::ostream& operator<<(std::ostream& out, Bark &object) {
	out << " produced by thread " << threadNumberToString(object.m_producer_thread_number, object.m_thread_map_width)
		<< " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, object.m_thread_map_width)
		<< " took " << std::setw(MAXIMUM_SLEEP_TIME_WIDTH) << object.m_consumer_sleep_time << " us, "
	    << "msg # " << std::setw(MESSAGE_NUMBER_WIDTH) << object.m_message_number << " \"" << *object.m_message << "\"";
	return out;
}



