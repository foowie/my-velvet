/* 
 * File:   BaseTypes.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:05
 */

#ifndef BASETYPES_H
#define	BASETYPES_H

#define BASE_A 0x0ULL // 00
#define BASE_C 0x1ULL // 01
#define BASE_G 0x2ULL // 10
#define BASE_T 0x3ULL // 11

namespace arg {
	namespace dna {
		typedef unsigned long long int Kmer;
	}
}

#endif	/* BASETYPES_H */

