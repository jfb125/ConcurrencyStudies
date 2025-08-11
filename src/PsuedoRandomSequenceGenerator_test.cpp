/*
 * PsuedoRandomSequenceGenerator_test.cpp
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */

#include "PsuedoRandomSequenceGenerator.h"

#include <memory>

void testPRSG() {

    unsigned num_frequencies = 65536;

    PsuedoRandomSequenceGenerator generator;
    generator.seed(0xffff).state(0xffff).polynomial(0xb400);
    std::cout << "generator initialized to: " << generator << std::endl;

    //  create an array of counts of each value in the sequence
    unsigned sequence_frequencies[num_frequencies];
    for (unsigned i = 0; i != num_frequencies; i++) {
        sequence_frequencies[i] = 0;
    }

    //  go through the sequence 'n' times to ensure that
    //    each location in the frequencies variable == num cycles
    unsigned num_cycles = 2;
    for (unsigned i = 0; i != num_cycles*(num_frequencies-1); i++) {
        sequence_frequencies[generator.state()]++;
        generator.next();
    }
#if 0
    unsigned _per_line = 64;
    unsigned _num_lines = 32;
    for (unsigned i = 0; i != _num_lines; i++) {
        std::cout << "0x" << std::setw(4) << std::hex << std::setfill('0') << (i*_per_line) << ": ";
        for (unsigned j = 0; j != _per_line; j++) {
        std::cout << std::setw(2) << std::dec << std::setfill(' ') << sequence_frequencies[(i*_per_line) + j] << " ";
        }
        std::cout << std::endl;
    }
#endif
    for (unsigned i = 0; i != num_frequencies; i++) {
        if (sequence_frequencies[i] != num_cycles) {
            std::cout << "0x" << std::setw(4) << std::hex << std::setfill('0')
                      << i << " is not " << num_cycles << ": "
                      << sequence_frequencies[i] << std::endl;
        }
    }

    unsigned max_modulo = 24;
    unsigned frequencies[max_modulo];
    std::cout << std::dec << std::setfill(' ');
    for (unsigned modulo = 1; modulo != max_modulo; modulo++) {
        //  zero the frequency counts
        for (unsigned i = 0; i != max_modulo; i++) {
            frequencies[i] = 0;
        }
        //  count the number of times a result occurs
        for (unsigned i = 1; i != num_frequencies; i++) {
            frequencies[i % modulo]++;
        }
        //  print out the counts
        std::cout << std::setw(4) << modulo << ": ";
        for (unsigned i = 0; i != modulo; i++) {
            std::cout << std::setw(5) << frequencies[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "testPRSG() exiting" << std::endl;
}
