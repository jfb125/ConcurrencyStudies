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

class RingBufferTestObject {
    int m_ID;

public:
    RingBufferTestObject() : m_ID(-1) {
        std::cout << "Default RingBufferTestObject constructor called" << std::endl;
    }
    RingBufferTestObject(int ID) : m_ID(ID) {
        std::cout << "RingBufferTestObject constructor called with ID: " << m_ID << std::endl;
    }
    virtual ~RingBufferTestObject() {
        std::cout << "RingBufferTestObject destructor called on object with ID " << m_ID << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& out, const RingBufferTestObject& object) {
       out << "ID: " << object.m_ID;
       return out;
    }
};

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
	std::unique_ptr<T[]> m_ring;
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
	    if (m_ring) {
	        m_ring.reset();
	    }
	    m_ring          = nullptr;
	    m_initialized   = false;
	}

public:

	T dequeue() {
	    if (m_initialized) {
            m_full = false;
            if (!m_empty) {
                size_t location = m_dequeue;
                if(++m_dequeue == m_size)
                    m_dequeue = 0;
                if (m_dequeue == m_enqueue) {
                    m_empty = true;
                }
                return m_ring[location];
            } else {
                m_under_run = true;
                // TODO - throw UNDERRUN exception
            }
	    } else {
	        // TODO - throw UNINITIALIZED exception
	    }
	}

	bool enqueue(T p_object) {
	    if (m_initialized) {
	        m_empty = false;
            if (m_full) {
                m_over_run = true;
                //  TODO - throw OVERRUN exception
                return false;
            }
            m_ring[m_enqueue] = p_object;
            if (++m_enqueue == m_size)
                m_enqueue = 0;
            if (m_enqueue == m_dequeue) {
                m_full = true;
            }
            return true;
	    } else {
	        // TODO - throw UNINITIALIZED exception
	        return false;
	    }
	}

	bool isEmpty() const    { return m_empty; }
	bool isFull() const     { return m_full; }
	bool isOverRun() const  { return m_over_run; }
	bool isUnderRun() const { return m_under_run; }
	bool isError() const    { return RingBufferError::NO_ERROR != m_error_state;  }
	int  errorState() const { return m_error_state; }
	bool flush() {
	    if (m_initialized) {
	        m_error_state   = RingBufferError::NO_ERROR;
            m_enqueue       = 0;
            m_dequeue       = 0;
            m_empty         = true;
            m_full          = false;
            m_over_run      = false;
            m_under_run     = false;
	    } else {
	        m_ring  = nullptr;
            toUninitialized();
            // TODO - throw uninitialized error
        }
        return m_initialized;
	}

	std::string to_string() const {

	    std::stringstream result;
	    if(m_initialized) {
            result    << "flags: "
                      << " FULL_" << (m_full ? "TRUE " : "false")
                      << " EMPTY_" << (m_empty ? "TRUE " : "false")
                      << " OVERRUN_" <<  (m_over_run ? "TRUE " : "false")
                      << " UNDERRUN_" << (m_under_run ? "TRUE " : "false")
                      << std::endl;
            result    << "error state: " << m_error_state << std::endl;
            for (size_t i = 0; i != m_size; i++) {
                result << std::setw(5) << i << ": ";
                if (i == m_enqueue) {
                    result << " nq ";
                } else {
                    result << "   ";
                }
                if (i == m_dequeue) {
                    result << " dq ";
                } else {
                    result << "    ";
                }
                result << "\n";
            }
	    } else {
	        std::cout << "ring buffer uninitialized flag is TRUE" << std::endl;
	    }
	    return result.str();
	}

	RingBuffer() = delete;
	RingBuffer(size_t size) {
	    m_ring = nullptr;
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
            m_ring          = std::make_unique<T[]>(m_size);
            m_initialized   = true;
	    } else {
	        // TODO - throw uninitialized error
	    }
	}

	virtual ~RingBuffer() {
	}
#if 0
	RingBuffer(const RingBuffer &other) {
	    if (this != &other) {
	        m_ring          = nullptr;
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        if (other.m_ring) {
	            m_ring = new T[m_size];
	            for (size_t i = 0; i != m_size; i++) {
	                m_ring[i] = other.m_ring[i];
	            }
	            m_initialized   = other.m_initialized;
	        } else {
	            m_initialized = false;
	        }
	    }
	}

	RingBuffer(RingBuffer &&other) {
	    if (this != &other) {
	        m_ring  = nullptr;
	        toUninitialized();
	        m_error_state   = other.m_error_state;
	        m_size          = other.m_size;
	        m_enqueue       = other.m_enqueue;
	        m_dequeue       = other.m_dequeue;
	        m_empty         = other.m_empty;
	        m_full          = other.m_full;
	        m_over_run      = other.m_over_run;
	        m_under_run     = other.m_under_run;
	        m_ring          = std::move(other.m_ring);
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
	        if (other.m_ring) {
	            m_ring      = new T[m_size];
	            for (size_t i = 0; i != m_size; i++) {
	                m_ring[i] = other.m_ring[i];
	            }
	            m_initialized = other.m_initialized;
	        } else {
	            m_initialized = false;
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
	        if (other.m_ring) {
	            m_ring          = std::move(other.m_ring);
	            m_initialized = true;
	        } else {
	            m_ring = nullptr;
	            m_initialized = false;
	        }
	    }
	    return *this;
	}
#endif
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
