/*
 * PsuedoRandomSequenceGenerator.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */

#ifndef PSUEDORANDOMSEQUENCEGENERATOR_H_
#define PSUEDORANDOMSEQUENCEGENERATOR_H_

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <sstream>

using PSRG_State_t = uint32_t;

constexpr PSRG_State_t PSRG_Default_Seed = 0xffff;
constexpr PSRG_State_t PSRG_Illegal_Seed = 0;
constexpr PSRG_State_t PSRG_Default_Polynomial = 0xb400;

class PsuedoRandomSequenceGenerator {
    PSRG_State_t _state;
    PSRG_State_t _seed;
    PSRG_State_t _polynomial;
public:
    PsuedoRandomSequenceGenerator();
    virtual ~PsuedoRandomSequenceGenerator();
    PsuedoRandomSequenceGenerator(const PsuedoRandomSequenceGenerator &other);
    PsuedoRandomSequenceGenerator(PsuedoRandomSequenceGenerator &&other);
    PsuedoRandomSequenceGenerator& operator=(
            const PsuedoRandomSequenceGenerator &other);
    PsuedoRandomSequenceGenerator& operator=(
            PsuedoRandomSequenceGenerator &&other);
    //  these do not restart the polynomial
    PSRG_State_t seed() const;
    PsuedoRandomSequenceGenerator& seed(PSRG_State_t new_seed);
    PSRG_State_t polynomial() const;
    PsuedoRandomSequenceGenerator& polynomial(PSRG_State_t new_polynomial);
    PSRG_State_t state() const;
    PsuedoRandomSequenceGenerator& state(PSRG_State_t new_state);
    void init();    // copies _seed into _state
    PSRG_State_t next();
    std::string to_string() const;
};

std::ostream& operator<<(std::ostream& out,
            const PsuedoRandomSequenceGenerator& object);

void testPRSG();

#endif /* PSUEDORANDOMSEQUENCEGENERATOR_H_ */
