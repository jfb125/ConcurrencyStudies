/*
 * SimpleRandomizer.h
 *
 *  Created on: Jun 13, 2025
 *      Author: joe
 */

/*	This class is a wrapper around an implementation found
 *	  on the internet.  The copyright for that implementation
 *	  is found at the bottom of this header
 */

#ifndef SIMPLERANDOMIZER_H_
#define SIMPLERANDOMIZER_H_

#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include <chrono>
#include <limits.h>

using ConcurrentRandomNumber = uint64_t;

#define SIMPLE_RANDOMIZER_DEFAULT_SEED 5489ULL

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL /* Least significant 31 bits */


class SimpleRandomizer {
private:
	ConcurrentRandomNumber m_seed;
	ConcurrentRandomNumber m_recent;
	ConcurrentRandomNumber mt[NN];
	int 	mti = NN+1;

	ConcurrentRandomNumber genrand64_int64(void);
	void init_genrand64(ConcurrentRandomNumber);


public:
	//	setting the seed does not re-initialize the machine
	//	user must call 'restart()' after setting a new seed
	SimpleRandomizer& seed(ConcurrentRandomNumber);	// does not reinitialize
	void restart(void);

	ConcurrentRandomNumber rand(void);
	// returns on interval [min, max) - i.e., not including max
	ConcurrentRandomNumber rand(ConcurrentRandomNumber min, ConcurrentRandomNumber max);

	ConcurrentRandomNumber recent(void) const;
	ConcurrentRandomNumber seed(void) const;

	// constructors initializes the machine
	SimpleRandomizer();
	SimpleRandomizer(ConcurrentRandomNumber seed);
	virtual ~SimpleRandomizer();
	SimpleRandomizer(const SimpleRandomizer &other);
	SimpleRandomizer& operator=(const SimpleRandomizer &other);
};

/*
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

ConcurrentRandomNumber getChronoSeed();

#endif /* SIMPLERANDOMIZER_H_ */
