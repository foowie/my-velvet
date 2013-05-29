/* 
 * File:   cRoadmapBuilder.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:09
 */

#include <iostream>
#include "BaseTypes.h"
#include "cKmerOccurenceMap.h"
#include "cKmerBuilder.h"


#ifndef CROADMAPBUILDER_H
#define	CROADMAPBUILDER_H
namespace arg {
	namespace dna {

		class cRoadmapBuilder {
		private:
			cKmerBuilder * kmerBuilder;
			cKmerOccurenceMap * map;

			unsigned short offset;
			cKmerOccurencePosition * firstKmerOccurence;
			unsigned short foundLength;
			bool found;
			cKmerOccurencePosition * position;
		public:

			cRoadmapBuilder(cKmerBuilder & pKmerBuilder, cKmerOccurenceMap & pMap) :
			kmerBuilder(&pKmerBuilder), map(&pMap) {
				offset = 0;
				firstKmerOccurence = NULL;
				foundLength = 0;
				found = false;
			}

			void Build() {
				while (kmerBuilder->Next()) {
					if (kmerBuilder->GetLineChanged()) {
						ProcessEndline();
					}

					const Kmer kmer = kmerBuilder->GetKmer();
					position = map->GetPosition(kmer);

					if (position == NULL) {
						found = false;
						map->Add(kmer, kmerBuilder->GetCurrentLine().index, offset);
					} else {
						foundLength++;
						found = true;
					}
					bool continuesOnLastFoundKmer = foundLength > 1 && found && firstKmerOccurence->line == position->line && firstKmerOccurence->offset + foundLength - 1 == position->offset;
					if (found && foundLength > 1 && !continuesOnLastFoundKmer) {
						foundLength--;
						ProcessRoadmap();
						firstKmerOccurence = position;
						foundLength = 1;
					} else if (found && foundLength > 1 /* && continuesOnLastFoundKmer*/) {
					} else if (!found && foundLength > 0) {
						ProcessRoadmap();
						++offset;
						firstKmerOccurence = NULL;
						foundLength = 0;
					} else if (found /* && foundLength <= 1*/) {
						firstKmerOccurence = position;
					} else /* if !found && foundLength <= 1*/ {
						++offset;
						firstKmerOccurence = NULL;
						foundLength = 0;
					}
				}
				if (found) {
					ProcessRoadmap();
				}
			}
		private:

			inline void ProcessEndline() {
				if (found) {
					ProcessRoadmap();
				}
				found = false;
				foundLength = 0;
				offset = 0;
				firstKmerOccurence = NULL;
				printf("ROADMAP %u\n", kmerBuilder->GetCurrentLine().index);
			}

			inline void ProcessRoadmap() {
				printf("%u\t%u\t%u\t%u\n", firstKmerOccurence->line, offset, firstKmerOccurence->offset, firstKmerOccurence->offset + foundLength);
			}
		};
	}
}

#endif	/* CROADMAPBUILDER_H */

