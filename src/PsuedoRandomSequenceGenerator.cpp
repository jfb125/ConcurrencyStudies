/*
 * PsuedoRandomSequenceGenerator.cpp
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */

#include "PsuedoRandomSequenceGenerator.h"

using Gen = PsuedoRandomSequenceGenerator;

Gen::PsuedoRandomSequenceGenerator() {
    _state      = PSRG_Default_Seed;
    _seed       = PSRG_Default_Seed;
    _polynomial = PSRG_Default_Polynomial;
}

Gen::~PsuedoRandomSequenceGenerator() {

}

Gen::PsuedoRandomSequenceGenerator(
        const PsuedoRandomSequenceGenerator &other) {
    _state      = other._state;
    _seed       = other._seed;
    _polynomial = other._polynomial;
}

Gen::PsuedoRandomSequenceGenerator(
        PsuedoRandomSequenceGenerator &&other) {
    _state      = other._state;
    _seed       = other._seed;
    _polynomial = other._polynomial;
}

PsuedoRandomSequenceGenerator& Gen::operator=(
        const PsuedoRandomSequenceGenerator &other) {
    _state      = other._state;
    _seed       = other._seed;
    _polynomial = other._polynomial;
    return *this;
}

PsuedoRandomSequenceGenerator& Gen::operator=(
        PsuedoRandomSequenceGenerator &&other) {
    _state      = other._state;
    _seed       = other._seed;
    _polynomial = other._polynomial;
    return *this;
}

PSRG_State_t Gen::seed() const          {   return _seed;       }
PSRG_State_t Gen::state() const         {   return _state;      }
PSRG_State_t Gen::polynomial() const    {   return _polynomial; }

PsuedoRandomSequenceGenerator& Gen::seed(PSRG_State_t new_seed) {
    _seed   = new_seed;
    return *this;
}
PsuedoRandomSequenceGenerator& Gen::state(PSRG_State_t new_state) {
    _state   = new_state;
    return *this;
}
PsuedoRandomSequenceGenerator& Gen::polynomial(PSRG_State_t new_polynomial) {
    _polynomial   = new_polynomial;
    return *this;
}
void Gen::init() {
    _state = _seed;
}
PSRG_State_t Gen::next() {
    if (_state & 1) {
        _state >>= 1;
        _state ^= _polynomial;
    } else {
        _state >>=1;
    }
    return _state;
}

std::string Gen::to_string() const {
    std::stringstream result;
    result << "seed: 0x" << std::setw(4) << std::hex << std::setfill('0') << _seed
           << " polynomial: 0x" << std::setw(4) << std::hex << std::setfill('0') << _polynomial
           << " state: 0x" << std::setw(4) << std::hex << std::setfill('0') << _state;
    return result.str();
}

std::ostream& operator<<(std::ostream& out,
            const PsuedoRandomSequenceGenerator& object) {
    out << object.to_string();
    return out;
}

