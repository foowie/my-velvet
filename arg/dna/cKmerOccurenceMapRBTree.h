/* 
 * File:   cKmerOccurenceMapRBTree.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:07
 */

#include "BaseTypes.h"
#include "cKmerOccurenceMap.h"

#ifndef CKMEROCCURENCEMAPRBTREE_H
#define	CKMEROCCURENCEMAPRBTREE_H

namespace arg {
	namespace dna {

		class cKmerOccurenceMapRBTree : public cKmerOccurenceMap {
		private:
			typedef std::map<Kmer, cKmerOccurencePosition*> KmersType;
			arg::cHugeArray<cKmerOccurencePosition> occurences;
			cKmerOccurencePosition positionTemp;
			KmersType kmers;
		public:

			inline cKmerOccurencePosition * GetPosition(const Kmer & kmer) {
				return kmers[kmer];
			}

			inline void Add(const Kmer & kmer, const unsigned int line, const unsigned short offset) {
				positionTemp.line = line;
				positionTemp.offset = offset;
				occurences.Add(&positionTemp);
				kmers[kmer] = &occurences[occurences.Count() - 1];
			}

		};

	}
}

#endif	/* CKMEROCCURENCEMAPRBTREE_H */

