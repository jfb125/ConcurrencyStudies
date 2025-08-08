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
#include <format>
#include <cctype>


#pragma push_macro("_ctoi")
#define _ctoi(n)	((n)-'0')

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

std::string threadNumberToString(int thread_number, int thread_number_width, int number_of_threads);
std::ostream& operator<<(std::ostream& out, Bark &object);

/*
 * 	m?:?	thread map parameters
 * 	i.e. - m1:8  for  8 threads 0
 * 								 1
 * 								  etc
 * 		   m2:12 for 12 threads 00
 * 		   						  01
 * 		   						    etc
 * 		   m3:12 for 12 threads 00
 * 		   						   01
 * 		   						   	  etc
 */

template<>
struct std::formatter<Bark> {

	int thread_number_width	= 0;
	int number_of_threads	= 0;

	enum class BarkParserState {
		IDLE,
		GET_THREAD_NUMBER_WIDTH,
		GET_NUMBER_OF_THREADS
	};

	template<class ParseContext>
	constexpr auto parse(ParseContext& ctx) {

		thread_number_width	= 0;
		number_of_threads	= 0;

		auto it = ctx.begin();
		if (it == ctx.end()) {
			return it;
		}

		BarkParserState parser_state = BarkParserState::IDLE;
		while (*it != '}') {
			switch(parser_state) {
			case BarkParserState::IDLE:
				switch((*it)) {
				case 'm':
				case 'M':
					parser_state = BarkParserState::GET_THREAD_NUMBER_WIDTH;
					thread_number_width = 0;
					break;
				default:
					break;
				}
				it++;
				break;
			case BarkParserState::GET_THREAD_NUMBER_WIDTH:
				if (isdigit(*it)) {
					thread_number_width += 10 * thread_number_width + _ctoi(*it);
				} else {
					if (*it == ':') {
						number_of_threads = 0;
						parser_state = BarkParserState::GET_NUMBER_OF_THREADS;
					} else {
						parser_state = BarkParserState::IDLE;
					}
				}
				it++;
				break;
			case BarkParserState::GET_NUMBER_OF_THREADS:
				if (isdigit(*it)) {
					number_of_threads = 10 * number_of_threads + _ctoi(*it);
				} else {
					parser_state = BarkParserState::IDLE;
				}
				it++;
				break;
			default:
				it++;
				break;
			}
		}
		return it;
	}

	template<class FmtContext>
	auto format(Bark& bark, FmtContext& ctx) const {
		return std::format_to(ctx.out(), "{}", threadNumberToString(bark.m_producer_thread_number, thread_number_width, number_of_threads));
	}
};

#pragma pop_macro("_ctoi")

#endif /* BARK_H_ */
