/*
 * CircularQueue_test.cpp
 *
 *  Created on: Aug 6, 2025
 *      Author: joe
 */

#include "RingBuffer.h"

void testRingBuffer() {

    RingBuffer<std::shared_ptr<RingBufferTestObject>> pointer_buffer(4);
    RingBuffer<RingBufferTestObject> object_buffer(2);
    std::shared_ptr<RingBufferTestObject> p_object = std::make_shared<RingBufferTestObject>(2);
    pointer_buffer.enqueue(std::make_shared<RingBufferTestObject>(1));
    pointer_buffer.enqueue(p_object);
    object_buffer.enqueue(RingBufferTestObject(10));
    std::cout << "pointer_buffer: " << std::endl << pointer_buffer.to_string() << std::endl;
    std::cout << "object_buffer:  " << std::endl << object_buffer.to_string() << std::endl;
    std::cout << "in testRingBuffer()" << std::endl;
    std::cout << "returning from testRingBuffer()" << std::endl;
}
