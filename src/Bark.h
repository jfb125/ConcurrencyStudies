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
#include <ctype.h>

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

std::string threadNumberToString(int thread_number, int thread_map_width, int number_of_threads);
std::ostream& operator<<(std::ostream& out, Bark &object);

/*
 * 	m2:12		-- map width {individual character:total width
 */
template<>
struct std::formatter<Bark> {

	int number_of_threads = 0;
	int thread_width      = 0;

	enum class BarkParserState {
		IDLE,
		THREAD_MAP_START,
		THREAD_MAP_GET_FIRST_NUMBER,
		THREAD_MAP_FIND_COLON,
		THREAD_MAP_GET_SECOND_NUMBER
	};
	template<class ParseContext>
	constexpr auto parse(ParseContext& ctx) {

		number_of_threads 	= 0;
		thread_width	 	= 0;

		auto it = ctx.begin();
		if (it == ctx.end()) {
			return it;
		}

		BarkParserState parser_state = BarkParserState::IDLE;
		while (*it != '}') {
			switch(parser_state) {
			case BarkParserState::IDLE:
				switch(*it) {
				case 'm':
					parser_state = BarkParserState::THREAD_MAP_START;
					break;
				default:
					break;
				}
			}
			case BarkParserState::THREAD_MAP_START:
				number_of_threads = 0;
				thread_width	  = 0;
				if (isalnum(*it) {

				}
			it++;
		}
		return it;
	}

	template<class FmtContext>
	auto format(Bark& bark, FmtContext& ctx) const {
		if (quoted) {
			return std::format_to(ctx.out(), " {} ", bark.m_consumer_sleep_time);
		} else {
			return std::format_to(ctx.out(), "\"{}\"", bark.m_consumer_sleep_time);
		}
	}
};


#endif /* BARK_H_ */
