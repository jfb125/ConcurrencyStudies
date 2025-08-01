/*
 * ConcurrencyStudies.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <thread>
#include <stdint.h>
#include <vector>
#include <numeric>
#include <utility>

#include "ConcurrencyStudies.h"

void	testThreadsAndTasks();

constexpr int message_number_width  = 4;
constexpr int thread_number_width 	= 10;

/* **************************************************************************** */
/* **************************************************************************** */
/* 							object to be enqueued / dequeued					*/
/* **************************************************************************** */
/* **************************************************************************** */

class Bark {
public:
	int m_thread_number;
	int m_message_number;
	std::string *m_message;

	Bark(int thread_number, int message_number, std::string *message) :
		m_thread_number(thread_number),
		m_message_number(message_number),
		m_message(message) {}

};

std::ostream& operator<<(std::ostream& out, Bark &object) {
	for (int i = 0; i != thread_number_width; i++) {
		if (i == object.m_thread_number) {
			out << object.m_thread_number;
		} else {
			out << " ";
		}
	}
	out << ", " << std::setw(message_number_width) << object.m_message_number << ": " << *object.m_message;
	return out;
}


/* **************************************************************************** */
/* **************************************************************************** */
/* 							producer function									*/
/* **************************************************************************** */
/* **************************************************************************** */

class ProducerThreadArgs {
private:
	ProducerThreadArgs() {}
public:
	std::string *id_msg;
	std::string *exit_msg;
	int thread_number;
	int repeat_count;
	std::shared_ptr<Bark*> *produced_queue;
	ProducerThreadArgs(std::string *_id_msg,
					   std::string *_exit_msg,
					   int _thread_number,
					   int _repeat_count,
					   std::shared_ptr<Bark*> * _produced_queue) :
							   id_msg(_id_msg),
							   exit_msg(_exit_msg),
							   thread_number(_thread_number),
							   repeat_count(_repeat_count),
							   produced_queue(_produced_queue) {}
};

void produceBark(ProducerThreadArgs *args, int &enqueue, int &active_thread_count, std::mutex &lock) {
	Bark* tmp;
	for (int i = 0; i != args->repeat_count; i++) {
		tmp = new Bark(args->thread_number, i, args->id_msg);
		lock.lock();
		args->produced_queue[enqueue] = std::make_shared<Bark*>(tmp);
		enqueue++;
		lock.unlock();
	}
	tmp = new Bark(args->thread_number, args->repeat_count, args->exit_msg);
	lock.lock();
	args->produced_queue[enqueue] = std::make_shared<Bark*>(tmp);
	enqueue++;
	active_thread_count--;
	lock.unlock();
}

/* **************************************************************************** */
/* **************************************************************************** */
/* 							consumer task										*/
/* **************************************************************************** */
/* **************************************************************************** */

void consumeBark(int &consumed_bark_count, std::shared_ptr<Bark*> *produced_q, std::shared_ptr<Bark*> *retired_q, int &nq, int &tcnt) {
	int dq = 0;
	while (tcnt != 0) {
		if (dq != nq) {
			consumed_bark_count++;
			*retired_q[dq] = *produced_q[dq];
			dq++;
		}
	}
	while (dq != nq) {
		consumed_bark_count++;
		*retired_q[dq] = *produced_q[dq];
		dq++;
	}
}


/* **************************************************************************** */
/* **************************************************************************** */
/* 							 		main function								*/
/* **************************************************************************** */
/* **************************************************************************** */

int main (int argc, char *argv[]) {

	std::cout << "ConcurrencyStudies.cpp revision " << REVISION_STRING
			  << " built on " << __DATE__ << " at " << __TIME__ << std::endl;

	std::mutex producer_lock;

	int		active_thread_count;
	int 	enqueue;
	int 	dequeue;

	int number_of_threads = 8;
	int number_of_tests = 1;
	uint64_t repeat_per_thread = 100;

	int consumed_string_count;

	for (int rep_count = 0, line_count = 0; rep_count != number_of_tests; rep_count++, line_count++) {
		// number of times a string will be enqueued / thread * number_of_threads +
		//	one more for the "exiting thread" message from each queue
		int queue_size = repeat_per_thread * number_of_threads + number_of_threads;
		std::shared_ptr<Bark*> 	produced_queue[queue_size];
		std::shared_ptr<Bark*> 	retired_queue[queue_size];
//		produced_queue = new Bark*[queue_size];
//		retired_queue = new Bark*[queue_size];
		enqueue = 0;
		dequeue = 0;
		active_thread_count = number_of_threads;
		consumed_string_count = 0;
		std::string *id_msg 	= new std::string("    barks   ");
		std::string *exit_msg 	= new std::string(" ***EXITS***");

		std::thread consumer_thread =
				std::thread(consumeBark,
								std::ref(consumed_string_count),
								produced_queue,
								retired_queue,
								std::ref(enqueue),
								std::ref(active_thread_count));

		// void produceBark(std::string *id_msg, std::string *exit_msg, int thread_number, int repeat_count, Bark **q, int &enq, int &tcnt) {
		std::vector<std::thread> producer_threads;
		std::vector<ProducerThreadArgs*> producers_args;
		for (int thread_number = 0; thread_number != number_of_threads; thread_number++) {
			ProducerThreadArgs *producer_args = new ProducerThreadArgs(id_msg, exit_msg, thread_number, repeat_per_thread, produced_queue);
			producers_args.push_back(producer_args);
			producer_threads.push_back(std::thread(produceBark, producer_args, std::ref(enqueue), std::ref(active_thread_count), std::ref(producer_lock)));
		}

		for (int i = 0; i != number_of_threads; i++) {
			if (producer_threads[i].joinable())
				producer_threads[i].join();
		}

		if (consumer_thread.joinable())
			consumer_thread.join();

		std::cout << "enqueued strings: " << enqueue << " vs dequeued strings " << dequeue << std::endl;
		std::cout << consumed_string_count << " strings were consumed in this order: " << std::endl;

		int out_of_order_count = 0;
		std::cout << std::setw(4) << "0" << ": " << *retired_queue[0] << std::endl;

		for (int i = 1; i != consumed_string_count; i++) {
			std::cout << std::setw(4) << i << ": " << *retired_queue[i];
			if (retired_queue[i]->m_thread_number != retired_queue[i-1]->m_thread_number) {
				std::cout << " !!!! OUT OF ORDER !!!!";
				out_of_order_count++;
			}
			std::cout << std::endl;
		}
		std::cout << "out of order count: " << out_of_order_count << std::endl;

		delete[] produced_queue;
		delete[] retired_queue;
		std::cout << std::endl;
	}

	//	testThreadsAndTasks();
	std::cout << "ConcurrencyStudies.cpp exiting" << std::endl;
	return EXIT_SUCCESS;
}

void announceResult(int number_of_threads, uint64_t sum, std::chrono::milliseconds duration) {
	std::cout << "\t" << "number of threads: " << std::setw(3) << number_of_threads
			  << " result: " << std::setw(24) << sum
			  << " elapsed time: " << std::setw(5) << duration.count() << "ms"
			  << std::endl;
}

void testThreadsAndTasks() {

	constexpr uint64_t number_of_elements = 1000 * 1000 * 1000;
	int _num_threads[] = { 1, 23, 211, 256 };
	int _num_num_threads = sizeof(_num_threads) / sizeof(int);
	TestRange number_of_threads(_num_threads, _num_num_threads);
	sumUsingFunction(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingFunctor(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingLambda(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncFunction(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncFunctor(number_of_elements, number_of_threads);
	number_of_threads.reset();
	sumUsingAsyncLambda(number_of_elements, number_of_threads);
	std::cout << std::endl << std::endl;
}
