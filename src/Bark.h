/*
 * Bark.h
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#ifndef BARK_H_
#define BARK_H_

#include <iostream>
#include <iomanip>
#include <memory>
#include <fmt/format.h>

#define MESSAGE_NUMBER_WIDTH	 3
#define THREAD_NUMBER_MAP_WIDTH	10
#define INVALID_THREAD_NUMBER -1
#define MAXIMUM_SLEEP_TIME	100
#define MAXIMUM_SLEEP_TIME_WIDTH 3

/* **************************************************************************** */
/* **************************************************************************** */
/* 							object to be enqueued / dequeued					*/
/* **************************************************************************** */
/* **************************************************************************** */

class Bark {
public:
	int m_thread_map_width;	// used for pretty printing
	int m_producer_thread_number;
	int m_message_number;
	int m_consumer_sleep_time;
	std::shared_ptr<std::string> m_message;
	int m_invalid_thread_number;
	int m_consumer_thread_number;

	Bark(int thread_map_width, int producer_thread_number, int message_number, int sleep_time, std::shared_ptr<std::string> message) :
		m_thread_map_width(thread_map_width),
		m_producer_thread_number(producer_thread_number),
		m_message_number(message_number),
		m_consumer_sleep_time(sleep_time),
		m_message(message) {
			m_invalid_thread_number 	= INVALID_THREAD_NUMBER;
			m_consumer_thread_number 	= m_invalid_thread_number;
	}

	~Bark() {
//		std::cout << "destroying " << m_producer_thread_number << ", " << m_message_number << std::endl;
	}
};

//	prints out a single digit thread number in a field "   3    " or "0      " or "       7"
//	which makes looking at a dump of the queues a little more intuitive:
//	   3       " some message "
//	0          " some other message "
//         7   " some other other message "

std::string threadNumberToString(int thread_number, int thread_map_width);
std::ostream& operator<<(std::ostream& out, Bark &object);

#endif /* BARK_H_ */
