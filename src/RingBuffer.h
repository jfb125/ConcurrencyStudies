/*
 * CircularQueue.h
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <iostream>
#include <iomanip>
#include <atomic>
#include <thread>
#include <memory>
#include <cstddef>
#include <utility>

namespace RingBufferError {
    constexpr int NO_ERROR = 0;
    constexpr int OVER_WRITE = 2;
    constexpr int UNDER_RUN = 1;
    constexpr int UNINITIALIZED = 4;
}
#if 0
enum class RingBufferError {
	NO_ERROR        = 0,
	UNDER_RUN       = 1,
	OVER_WRITE      = 2,
	UNINITIALIZED   = 4
};
#endif

template <typename T>
class RingBuffer {
	int m_error_state;
	size_t m_size;
	std::atomic<size_t> m_enqueue;
	std::atomic<size_t> m_dequeue;
	std::atomic<bool> m_empty;
	std::atomic<bool> m_full;
	std::atomic<bool> m_over_run;
	std::atomic<bool> m_under_run;
	std::unique_ptr<std::shared_ptr<T>[]> m_queue;
	bool m_initialized;

	void toUninitialized() {
	    m_error_state   = RingBufferError::UNINITIALIZED;
	    m_size          = 0;
	    m_enqueue       = 0;
	    m_dequeue       = 0;
	    m_empty         = false;
	    m_full          = false;
	    m_over_run      = false;
	    m_under_run     = false;
	    if (m_queue) {
	        m_queue.reset();
	    }
	    m_queue         = nullptr;
	    m_initialized   = false;
	}

public:

	std::shared_ptr<T> dequeue() {
	    std::shared_ptr<T> result = nullptr;
	    if (m_initialized) {
            m_full = false;
            if (!m_empty) {
                result = m_queue[m_dequeue];
                if(++m_dequeue == m_size)
                    m_dequeue = 0;
                if (m_dequeue == m_enqueue) {
                    m_empty = true;
                } else {
                    m_under_run = true;
                }
            }
	    }
	    return result;
	}

	bool enqueue(std::shared_ptr<T> p_object) {
	    if (m_initialized) {
	        m_empty = false;
            if (m_full) {
                m_over_run = true;
                return false;
            }
            m_queue[m_enqueue] = p_object;
            if (++m_enqueue == m_size)
                m_enqueue = 0;
            if (m_enqueue == m_dequeue) {
                m_full = true;
            }
            return true;
	    } else {
	        // TODO - throw exception
	        return false;
	    }
	}

	bool isEmpty() const    { return m_empty; }
	bool isFull() const     { return m_full; }
	bool isOverRun() const  { return m_over_run; }
	bool isUnderRun() const { return m_under_run; }
	bool isError() const {   return RingBufferError::NO_ERROR != m_error_state;  }
	int  errorState() const { return m_error_state; }
	bool flush() {
        if (m_size != 0) {
            m_error_state   = RingBufferError::NO_ERROR;
            m_enqueue       = 0;
            m_dequeue       = 0;
            m_empty         = true;
            m_full          = false;
            m_over_run      = false;
            m_under_run     = false;
            m_queue         = std::make_unique<std::shared_ptr<T>>(new std::shared_ptr<T>[m_size]);
            for (int i = 0; i != m_size; i++)
                m_queue[i] = nullptr;
            m_initialized   = true;
        } else {
            toUninitialized();
            // TODO - throw uninitialized error
        }
        return m_initialized;
	}

	std::string to_string() const {

	    std::stringstream result;
	    if(m_initialized) {
            result    << "flags: "
                      << " full: " << (m_full ? "TRUE " : "false")
                      << " empty: " << (m_empty ? "TRUE " : "false")
                      << " overrun: " <<  (m_over_run ? "TRUE " : "false")
                      << " underrun: " << (m_under_run ? "TRUE " : "false")
                      << std::endl;
            result    << "error state: " << m_error_state << std::endl;
            for (size_t i = 0; i != m_size; i++) {
                result << std::setw(5) << i << ": ";
                if (i == enqueue) {
                    result << " nq ";
                } else {
                    result << "   ";
                }
                if (i == dequeue) {
                    result << " dq ";
                } else {
                    result << "    ";
                }
                if (m_queue[i]) {
                    result << *(m_queue[i]) << std::endl;
                } else {
                    result << "<nullptr>" << std::endl;
                }
            }
	    } else {
	        std::cout << "ring buffer uninitialized flag is TRUE" << std::endl;
	    }
	    return result.str();
	}

	RingBuffer() = delete;
	RingBuffer(size_t size) {
        toUninitialized();
	    if (size != 0) {
            m_error_state   = RingBufferError::NO_ERROR;
            m_size          = size;
            m_enqueue       = 0;
            m_dequeue       = 0;
            m_empty         = true;
            m_full          = false;
            m_over_run      = false;
            m_under_run     = false;
            m_queue         = std::make_unique<std::shared_ptr<T>>(new std::shared_ptr<T>[m_size]);
            for (int i = 0; i != m_size; i++)
                m_queue[i] = nullptr;
            m_initialized = true;
	    } else {
	        // TODO - throw uninitialized error
	    }
	}

	virtual ~RingBuffer() {

	}

	RingBuffer(const RingBuffer &other) {
	    if (this != &other) {
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        m_initialized = other.m_initialized;
	        m_queue         = std::make_unique<std::shared_ptr<T>>(new std::shared_ptr<T>[m_size]);
	        for (int i = 0; i != m_size; i++) {
	            m_queue[i] = std::make_shared<T>(other.m_queue[i]);
	        }
	    }
	}

	RingBuffer(RingBuffer &&other) {
	    if (this != &other) {
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        m_queue         = std::move(other.m_queue);
	        other.toUninitialized();
	    }
	}

	RingBuffer& operator=(const RingBuffer &other) {
	    if (this != &other) {
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        if (m_size != 0) {
	            m_initialized   = other.m_initialized;
	            m_queue         = std::make_unique<std::shared_ptr<T>>(new std::shared_ptr<T>[m_size]);
	            for (int i = 0; i != m_size; i++) {
	                m_queue[i] = std::make_shared<T>(other.m_queue[i]);
	            }
	        } else {
	            m_initialized   = false;
	            m_queue         = nullptr;
	        }
	    }
	    return *this;
	}

	RingBuffer& operator=(RingBuffer &&other) {
	    if (this != &other) {
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        if (m_size != 0) {
	            m_initialized   = other.m_initialized;
	            m_queue         = std::make_unique<std::shared_ptr<T>>(new std::shared_ptr<T>[m_size]);
	            for (int i = 0; i != m_size; i++) {
	                m_queue[i] = std::make_shared<T>(other.m_queue[i]);
	            }
	        } else {
	            m_initialized   = false;
	            m_queue         = nullptr;
	        }
	    }
	    return *this;
	}
};
#if 0
std::ostream& operator<<(std::ostream &out, RingBufferError error)
{
    if (error == RingBufferError::NO_ERROR) {
        out << "NO_ERROR";
    } else {
        if (error == RingBufferError::UNINITIALIZED) {
            out << "UNINITIALIZED";
        } else {
            if (error & RingBufferError::OVER_WRITE) {
                out << "OVER_WRITE";
                if (error & RingBufferError::UNDER_RUN) {
                    out << " and UNDER_RUN";
                }
            } else {
                if (error & RingBufferError::UNDER_RUN) {
                    out << "UNDER_RUN";
                }
            }
        }
    }
    return out;
}
#endif
template <typename T>
std::ostream &operator<<(std::ostream& out, const RingBuffer<T> &object) {
    out << object.to_string();
    return out;
}

void testRingBuffer();

#endif /* RINGBUFFER_H_ */
