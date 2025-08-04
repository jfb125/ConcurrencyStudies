/*
 * SimpleRandomizer.cpp
 *
 *  Created on: Jun 13, 2025
 *      Author: joe
 */

#include "SimpleRandomizer.h"

/* ************************************************************	*/
/*					initialization interface					*/
/* ************************************************************	*/

SimpleRandomizer& SimpleRandomizer::seed(uint64_t seed) {
	_seed = seed;
	return *this;
}

uint64_t SimpleRandomizer::seed(void) const {
	return _seed;
}

void SimpleRandomizer::restart(void) {
	init_genrand64(_seed);
	_recent = _seed;
}

/* ************************************************************	*/
/*					getting random numbers						*/
/* ************************************************************	*/

uint64_t SimpleRandomizer::rand(void) {

	_recent = genrand64_int64();
	return _recent;
}

//	returns on range [min, max), i.e. - not including max
uint64_t SimpleRandomizer::rand(uint64_t min, uint64_t max) {

	if (min >= max) {
		_recent = 0;
	} else {

	_recent = genrand64_int64();
	_recent %= max-min;
	_recent += min;
	}
	return _recent;
}

uint64_t SimpleRandomizer::recent(void) const {
	return _recent;
}

/* ****************************************************************	*/
/*					c'tor / d'tor / copy c'tor						*/
/* ****************************************************************	*/

SimpleRandomizer::SimpleRandomizer() {

	_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
	for (int i = 0; i != NN; i++)
		mt[i] = 0;
	mti = NN+1;
	init_genrand64(_seed);
}

SimpleRandomizer::SimpleRandomizer(uint64_t seed) {

	_seed = seed;
	for (int i = 0; i != NN; i++)
		mt[i] = 0;
	mti = NN+1;
	init_genrand64(_seed);
}

SimpleRandomizer::~SimpleRandomizer() {
}

SimpleRandomizer::SimpleRandomizer(const SimpleRandomizer &other) {

	if (&other == this)
		return;
	_recent = other._recent;
	_seed = other._seed;
	for (int i = 0; i != NN; i++) {
		mt[i] = other.mt[i];
	}
	mti = other.mti;
}

SimpleRandomizer& SimpleRandomizer::operator=(const SimpleRandomizer &other) {

	if (&other == this)
		return *this;

	_recent = other._recent;
	_seed = other._seed;
	for (int i = 0; i != NN; i++) {
		mt[i] = other.mt[i];
	}
	mti = other.mti;
	return *this;
}


/* ****************************************************************	*/
/*					The machine itself								*/
/* ****************************************************************	*/

/* initializes mt[NN] with a seed */
void SimpleRandomizer::init_genrand64(uint64_t seed)
{
    mt[0] = seed;
    for (mti=1; mti<NN; mti++)
        mt[mti] =  (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
}

/* generates a random number on [0, 2^64-1]-interval */
uint64_t SimpleRandomizer::genrand64_int64(void)
{
    int i;
    uint64_t x;
    static uint64_t mag01[2]={0ULL, MATRIX_A};

    if (mti >= NN) { /* generate NN words at one time */

        /* if init_genrand64() has not been called, */
        /* a default initial seed is used     */
    	/*	THIS WILL NEVER HAPPEN !!! */
        if (mti == NN+1) {
        	std::cout << "genrand64_int64() ERROR: generator not initialized" << std::endl;
            init_genrand64(_seed);
        }

        for (i=0;i<NN-MM;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        for (;i<NN-1;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        x = (mt[NN-1]&UM)|(mt[0]&LM);
        mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

        mti = 0;
    }

    x = mt[mti++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return x;
}

/*	**************************************************************	*/
/*						related functions							*/
/*	**************************************************************	*/

uint64_t getChronoSeed() {
	auto now = std::chrono::system_clock::now();
	auto duration_since_epoch = now.time_since_epoch();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
	long long milliseconds_count = milliseconds.count();
	return static_cast<unsigned long long>(milliseconds_count);
}
