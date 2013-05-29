/* 
 * File:   cKmerOccurenceMapUnorderedMap.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:07
 */

#include "BaseTypes.h"
#include "cKmerOccurenceMap.h"
#include <unordered_map>
#include "../core/cHugeArray.h"

#ifndef CKMEROCCURENCEMAPUNORDEREDMAP_H
#define	CKMEROCCURENCEMAPUNORDEREDMAP_H

namespace arg {
	namespace dna {

		class cKmerOccurenceMapUnorderedMap : public cKmerOccurenceMap {
		private:
			typedef std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash<Kmer >> KmersType;
			arg::cHugeArray<cKmerOccurencePosition> occurences;
			cKmerOccurencePosition positionTemp;
			std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash<Kmer >> *kmers;

		public:

			cKmerOccurenceMapUnorderedMap() {
				kmers = new std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash < Kmer >> (10000); /** @todo: heuristic? 1000 */
			}

			inline cKmerOccurencePosition * GetPosition(const Kmer & kmer) {
				return (*kmers)[kmer];
			}

			inline void Add(const Kmer & kmer, const unsigned int line, const unsigned short offset) {
				positionTemp.line = line;
				positionTemp.offset = offset;
				occurences.Add(&positionTemp);
				(*kmers)[kmer] = &occurences[occurences.Count() - 1];
			}

		};
	}
}

#endif	/* CKMEROCCURENCEMAPUNORDEREDMAP_H */

