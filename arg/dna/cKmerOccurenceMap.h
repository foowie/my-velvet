/* 
 * File:   cKmerOccurenceMap.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:02
 */

#include "BaseTypes.h"

#ifndef CKMEROCCURENCEMAP_H
#define	CKMEROCCURENCEMAP_H

namespace arg {
	namespace dna {

		struct cKmerOccurencePosition {
			unsigned int line;
			unsigned short offset;
		};

		class cKmerOccurenceMap {
		public:
			virtual cKmerOccurencePosition * GetPosition(const Kmer & kmer) = 0;
			virtual void Add(const Kmer & kmer, const unsigned int line, const unsigned short offset) = 0;
		};
	}
}

#endif	/* CKMEROCCURENCEMAP_H */

