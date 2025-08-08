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

#define INVALID_THREAD_NUMBER -1
#define MAXIMUM_SLEEP_TIME	100

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

std::string toString(Bark &object,
					 int producer_thread_number_width, int producer_number_of_threads,
					 int consumer_thread_number_width, int consumer_number_of_threads,
					 int sleep_time_width, int message_number_width);
std::string threadNumberToString(int thread_number, int thread_number_width, int number_of_threads);
std::ostream& operator<<(std::ostream& out, Bark &object);

/*
 * 	{ p?.? c?.? s? m? }
 * 	p?.?	producer thread map parameters
 * 	i.e. - p1:8  for  8 threads 0
 * 								 1
 * 								  etc
 * 		   p2:12 for 12 threads 00
 * 		   						  01
 * 		   						    etc
 * 		   p3:12 for 12 threads 00
 * 		   						   01
 * 		   						   	  etc
 *
 * 	c?.?	consumer map parameters
 * 	i.e. - c1:8  for  8 threads 0
 * 								 1
 * 								  etc
 * 		   c2:12 for 12 threads 00
 * 		   						  01
 * 		   						    etc
 * 		   c3:12 for 12 threads 00
 * 		   						   01
 * 		   						   	  etc
 *
 * 	s?		consumer sleep time width
 * 	m?		message number width
 */
#define PRODUCER_PARAM_CHAR_LOWER	'p'
#define PRODUCER_PARAM_CHAR_UPPER	'P'
#define CONSUMER_PARAM_CHAR_LOWER	'c'
#define CONSUMER_PARAM_CHAR_UPPER	'C'
#define SLEEP_TIME_WIDTH_PARAM_LOWER	's'
#define SLEEP_TIME_WIDTH_PARAM_UPPER	'S'
#define MESSAGE_NUMBER_WIDTH_PARAM_LOWER	'm'
#define MESSAGE_NUBMER_WIDTH_PARAM_UPPER	'M'

//	improves readability.  cannot be made into
//	  a function because it is a const char*
template<>
struct std::formatter<Bark> {

#define _parse_bark_param_value(_value, _it)\
	do {\
		_value = 0;\
		while (*_it != '}' && (*_it >= '0' && *_it <= '9')) {\
			_value = 10 * _value + (*_it-'0');\
		}\
	} while (false)

	int producer_thread_number_width	= 0;
	int number_of_producer_threads		= 0;
	int consumer_thread_number_width	= 0;
	int number_of_consumer_threads		= 0;
	int sleep_time_width				= 0;
	int message_number_width			= 0;

	enum class BarkParserState {
		IDLE,
		GET_PRODUCER_THREAD_NUMBER_WIDTH,
		GET_PRODUCER_NUMBER_OF_THREADS,
		GET_CONSUMER_THREAD_NUMBER_WIDTH,
		GET_CONSUMER_NUMBER_OF_THREADS,
		GET_SLEEP_TIME_WIDTH,
		GET_MESSAGE_NUMBER_WIDTH
	};

	template<class ParseContext>
	constexpr auto parse(ParseContext& ctx) {

		auto it = ctx.begin();
		if (it == ctx.end()) {
			return it;
		}

		BarkParserState parser_state = BarkParserState::IDLE;
		while (*it != '}') {
			switch(parser_state) {
			case BarkParserState::IDLE:
				switch((*it)) {
				case PRODUCER_PARAM_CHAR_LOWER:
				case PRODUCER_PARAM_CHAR_UPPER:
					parser_state = BarkParserState::GET_PRODUCER_THREAD_NUMBER_WIDTH;
					break;
				case CONSUMER_PARAM_CHAR_LOWER:
				case CONSUMER_PARAM_CHAR_UPPER:
					parser_state = BarkParserState::GET_CONSUMER_THREAD_NUMBER_WIDTH;
					break;
				case SLEEP_TIME_WIDTH_PARAM_LOWER:
				case SLEEP_TIME_WIDTH_PARAM_UPPER:
					parser_state = BarkParserState::GET_SLEEP_TIME_WIDTH;
					break;
				case MESSAGE_NUMBER_WIDTH_PARAM_LOWER:
				case MESSAGE_NUBMER_WIDTH_PARAM_UPPER:
					parser_state = BarkParserState::GET_MESSAGE_NUMBER_WIDTH;
					break;
				default:
					break;
				}
				it++;
				break;
			case BarkParserState::GET_PRODUCER_THREAD_NUMBER_WIDTH:
				_parse_bark_param_value(producer_thread_number_width, it);
				if (*it != '}') {
					it++;
					_parse_bark_param_value(number_of_producer_threads, it);
				}
				parser_state = BarkParserState::IDLE;
				break;
			case BarkParserState::GET_CONSUMER_THREAD_NUMBER_WIDTH:
				_parse_bark_param_value(consumer_thread_number_width, it);
				if (*it != '}') {
					it++;
					_parse_bark_param_value(number_of_consumer_threads, it);
				}
				parser_state = BarkParserState::IDLE;
				break;
			case BarkParserState::GET_SLEEP_TIME_WIDTH:
				_parse_bark_param_value(sleep_time_width, it);
				parser_state = BarkParserState::IDLE;
				break;
			case BarkParserState::GET_MESSAGE_NUMBER_WIDTH:
				_parse_bark_param_value(message_number_width, it);
				parser_state = BarkParserState::IDLE;
				break;
			default:
				it++;
				break;
			}
		}
		return it;
	}

/*
	std::ostream& operator<<(std::ostream& out, Bark &object) {
		out << " produced by thread " << threadNumberToString(object.m_producer_thread_number, object.m_thread_map_width, 1)
			<< " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, object.m_thread_map_width, 1)
			<< " took " << std::setw(MAXIMUM_SLEEP_TIME_WIDTH) << object.m_consumer_sleep_time << " us, "
		    << "msg # " << std::setw(MESSAGE_NUMBER_WIDTH) << object.m_message_number << " \"" << *object.m_message << "\"";
		return out;
	}
*/

	template<class FmtContext>
	auto format(Bark& object, FmtContext& ctx) const {
		std::stringstream result;
		result << " produced by thread " << threadNumberToString(object.m_producer_thread_number, producer_thread_number_width, number_of_producer_threads)
			   << " consumed by thread " << threadNumberToString(object.m_consumer_thread_number, producer_thread_number_width, number_of_producer_threads)
			   << " took " << std::setw(sleep_time_width) << object.m_consumer_sleep_time << " us, "
			   << "msg # " << std::setw(message_number_width) << object.m_message_number << "\"" << *object.m_message << "\"";
		return std::format_to(ctx.out(), "{}", result.str());
	}
};

#pragma pop_macro("_ctoi")

#endif /* BARK_H_ */
