/* 
 * File:   cKmerBuilder.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:09
 */

#include <iostream>
#include "BaseTypes.h"
#include "cDnaLine.h"
#include "cLoader.h"

#ifndef CKMERBUILDER_H
#define	CKMERBUILDER_H

namespace arg {
	namespace dna {

		class cKmerBuilder {
		private:
			cLoader * loader;
			cDnaLine * line;
			unsigned short offset;
			unsigned short kmerLength;
			bool lineChanged;
			Kmer nucleoideTable[256], strandNucleoideTable[256];
		public:

			cKmerBuilder(cLoader & loader, const unsigned short kmerLength) {
				this->loader = &loader;
				this->kmerLength = kmerLength;
				this->lineChanged = false;
				if (kmerLength > 32) {
					throw "Kmer length cant be longer than 32";
				}
				loader.Reset();
				nucleoideTable['N'] = nucleoideTable['n'] = BASE_A;
				nucleoideTable['A'] = nucleoideTable['a'] = strandNucleoideTable['T'] = strandNucleoideTable['t'] = BASE_A;
				nucleoideTable['C'] = nucleoideTable['c'] = strandNucleoideTable['G'] = strandNucleoideTable['g'] = BASE_C;
				nucleoideTable['G'] = nucleoideTable['g'] = strandNucleoideTable['C'] = strandNucleoideTable['c'] = BASE_G;
				nucleoideTable['T'] = nucleoideTable['t'] = strandNucleoideTable['A'] = strandNucleoideTable['a'] = BASE_T;
			};

			inline const unsigned short GetKmerLength() const {
				return kmerLength;
			};

			inline const unsigned short GetKmerCountInCurrentLine() const {
				return line->length - kmerLength + 1;
			}

			inline const cDnaLine & GetCurrentLine() const {
				return *line;
			}

			inline const unsigned short GetOffset() const {
				return offset;
			}

			inline const bool GetLineChanged() const {
				return lineChanged;
			}

			inline bool Next() {
				if (line == NULL || offset >= line->length - kmerLength) {
					offset = 0;
					lineChanged = true;
					do {
						if ((line = loader->ReadLine()) == NULL) {
							return false;
						}
					} while (line->length < kmerLength); /** @todo: when line is shorter, ROADMAP n-1 wont show */
				} else {
					lineChanged = false;
					offset++;
				}
				return true;
			}

			inline void DisplayKmer() const {
				for (unsigned short i = 0; i < kmerLength; i++) {
					printf("%c", line->sequence[i + offset]);
				}
			}

			inline const Kmer GetKmer() const {
				Kmer kmer = 0;
				for (unsigned short i = 0; i < kmerLength; i++) {
					kmer <<= 2;
					kmer |= nucleoideTable[line->sequence[i + offset]];
				}
				return kmer;
			}

			inline const Kmer GetStrandKmer() const {
				Kmer kmer = 0;
				for (unsigned short i = kmerLength - 1; i >= 0; i--) {
					kmer <<= 2;
					kmer |= strandNucleoideTable[line->sequence[i + offset]];
				}
				return kmer;
			}
		};
	}
}

#endif	/* CKMERBUILDER_H */

