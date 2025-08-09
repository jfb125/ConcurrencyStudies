/*
 * CircularQueue.h
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <iostream>
#include <atomic>
#include <thread>
#include <memory>

template <typename T>
class RingBuffer {
//	RingBufferError m_error_state;
	int	m_size;
	bool m_empty;
	bool m_full;
	bool m_is_initialized;
	std::unique_ptr<std::shared_ptr<T>> m_queue;
public:
	enum class RingBufferError : int;
	enum class RingBufferError {
		NO_ERROR        = 0,
		UNDERRUN        = 1,
		OVERWRITE       = 2,
		UNINITIALIZED   = 3
	};

	std::shared_ptr<T> dequeue();
	void enqueue(std::shared_ptr<T> object);
	bool isEmpty() const;
	bool isFull() const;
	RingBufferError isError() const;
	bool reset();

	RingBuffer();
	virtual ~RingBuffer();
	RingBuffer(const RingBuffer &other);
	RingBuffer(RingBuffer &&other);
	RingBuffer& operator=(const RingBuffer &other);
	RingBuffer& operator=(RingBuffer &&other);
};

#endif /* RINGBUFFER_H_ */
