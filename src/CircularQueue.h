/*
 * CircularQueue.h
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#ifndef CIRCULARQUEUE_H_
#define CIRCULARQUEUE_H_

#include <iostream>
#include <atomic>
#include <thread>
#include <memory>

template <typename T>
class CircularQueue {
	enum class CircularQueueError;
	int	m_size;
	bool m_empty;
	bool m_full;
	bool m_is_initialized;
	std::unique_ptr<std::shared_ptr<T>> m_queue;
	CircularQueueError m_error_state;
public:
	enum class CircularQueueError {
		NO_ERROR,
		UNDERRUN,
		OVERWRITE,
		UNINITIALIZED
	};

	std::shared_ptr<T> dequeue();
	void enqueue(std::shared_ptr<T> object);
	bool isEmpty() const;
	bool isFull() const;
	CircularQueueError isError() const;
	bool reset();

	CircularQueue();
	virtual ~CircularQueue();
	CircularQueue(const CircularQueue &other);
	CircularQueue(CircularQueue &&other);
	CircularQueue& operator=(const CircularQueue &other);
	CircularQueue& operator=(CircularQueue &&other);
};

#endif /* CIRCULARQUEUE_H_ */
