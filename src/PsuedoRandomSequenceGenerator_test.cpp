/*
 * PsuedoRandomSequenceGenerator_test.cpp
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */

#include "PsuedoRandomSequenceGenerator.h"

void testPRSG() {

    constexpr int num_frequencies = 65536;
    constexpr int modulo = 10;

    PsuedoRandomSequenceGenerator generator;
    generator.seed(0xffff).state(0xffff).polynomial(0xb400);
    std::cout << "generator initialized to: " << generator << std::endl;

    uint8_t sequence_frequencies[num_frequencies];
    for (int i = 0; i != num_frequencies; i++) {
        sequence_frequencies[i] = 0;
    }

    for (int i = 0; i != num_frequencies-1; i++) {
        sequence_frequencies[generator.state()]++;
        generator.next();
    }

    for (int i = 0; i != num_frequencies; i++) {
        if (sequence_frequencies[i] != 1) {
            std::cout << "0x" << std::setw(4) << std::hex << std::setfill('0')
                      << i << " is not 1: " << sequence_frequencies[i] << std::endl;
        }
    }

    int frequencies[modulo];
    for (int i = 0; i != modulo; i++) {
        frequencies[modulo] = 0;
    }
    for (int i = 1; i != num_frequencies; i++) {
        frequencies[i % modulo]++;
    }
    std::cout << std::dec << std::setfill(' ');

    for (int i = 0; i != modulo; i++) {
        std::cout << i << ": " << std::setw(5) << frequencies[i] << std::endl;
    }

    std::cout << "testPRSG() exiting" << std::endl;
}
