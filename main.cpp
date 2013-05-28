#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <iostream>
#include <bitset>
#include "Stopwatch.h"
#include <unordered_map>

#include "arg/core/cHugeArray.h"

#define KMER_LENGTH 5

namespace arg {
	namespace dna {

#define BASE_A 0x0ULL // 00
#define BASE_C 0x1ULL // 01
#define BASE_G 0x2ULL // 10
#define BASE_T 0x3ULL // 11

		typedef unsigned long long int Kmer;

		class cKmerOccurencePosition {
		public:
			unsigned int line;
			unsigned short offset;
		};

		class cKmerOccurenceMap {
		public:
			virtual cKmerOccurencePosition * GetPosition(const Kmer & kmer) = 0;
			virtual void Add(const Kmer & kmer, const unsigned int line, const unsigned short offset) = 0;
		};

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

		class cKmerOccurenceMapUnorderedMap : public cKmerOccurenceMap {
		private:
			typedef std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash<Kmer >> KmersType;
			arg::cHugeArray<cKmerOccurencePosition> occurences;
			cKmerOccurencePosition positionTemp;
			std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash<Kmer >> *kmers;
			
		public:

			cKmerOccurenceMapUnorderedMap() {
				kmers = new std::unordered_map<Kmer, cKmerOccurencePosition*, std::hash < Kmer >> (1000); /** @todo: heuristic? 1000 */
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

		class cDnaLine {
		public:
			unsigned int index;
			unsigned short length;
			char * sequence;
			char * text;

			cDnaLine(const unsigned short maxSequenceLength, const unsigned short maxTextLength) {
				sequence = new char[maxSequenceLength + 1];
				text = new char[maxTextLength + 1];
				index = 0;
			}

			cDnaLine(const cDnaLine & that) {
				index = that.index;
				length = that.length;

				sequence = new char[length + 1];
				strncpy(sequence, that.sequence, length + 1);

				text = new char[strlen(that.text) + 1];
				strcpy(text, that.text);
			}

			~cDnaLine() {
				delete[] sequence;
				delete[] text;
			}

			void DisplayLine() const {
				printf("%s - %u - %s\n", sequence, index, text);
			}


		};

		class cLoader {
		public:

			virtual ~cLoader() {
			};
			virtual cDnaLine * ReadLine() = 0;
			virtual void Reset() = 0;
			virtual void Close() = 0;
		protected:

			inline unsigned int RTrim(char * line) {
				unsigned int length = strlen(line) - 1;
				while (line[length] == '\n' || line[length] == '\r') {
					line[length] = '\0';
					length--;
				}
				return length + 1;
			}
		};

		class cFastQFileLoader : public cLoader {
		protected:
			FILE * file;
			cDnaLine * currentLine;
			char temp[65535];
		public:

			cFastQFileLoader(const char * fileName) {
				file = fopen(fileName, "rt");
				if (file == false) {
					throw "Error when opening file";
				}
				currentLine = new cDnaLine(65534, 65534);
			}

			~cFastQFileLoader() {
				delete currentLine;
			}

			cDnaLine * ReadLine() {
				if (fgets(currentLine->text, 65534, file) == NULL) {
					return NULL;
				}
				RTrim(currentLine->text);
				strcpy(currentLine->text, currentLine->text + 1); // remove @
				if (fgets(currentLine->sequence, 65534, file) == NULL) {
					return NULL;
				}
				currentLine->length = RTrim(currentLine->sequence);
				fgets(temp, 65534, file);
				if (temp[0] != '+') {
					throw "Invalid FastQ sequence";
				}
				fgets(temp, 64435, file);
				currentLine->index++;
				return currentLine;
			}

			void Reset() {
				throw "Reset is not implemented";
			}

			void Close() {
				if (file != NULL) {
					fclose(file);
					file = NULL;
				}
			}

		};

		class cKmerBuilder {
		private:
			cLoader * loader;
			cDnaLine * line;
			unsigned short offset;
			unsigned short kmerLength;
			bool lineChanged;
			Kmer nucleoideTable[256];
		public:

			cKmerBuilder(cLoader & loader, const unsigned short kmerLength) {
				this->loader = &loader;
				this->kmerLength = kmerLength;
				this->lineChanged = false;
				if (kmerLength > 32) {
					throw "Kmer length cant be longer than 32";
				}
				nucleoideTable['N'] = nucleoideTable['n'] = BASE_A;
				nucleoideTable['A'] = nucleoideTable['a'] = BASE_A;
				nucleoideTable['C'] = nucleoideTable['c'] = BASE_C;
				nucleoideTable['G'] = nucleoideTable['g'] = BASE_G;
				nucleoideTable['T'] = nucleoideTable['t'] = BASE_T;
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
		};

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

class FastQLine {
public:
	unsigned int index;
	char text[200];
	char sequence[200];
	char quality[200];
	unsigned int length;

	inline void DisplayLine(FastQLine * line) {
		printf("@%s\n%s\n+\n%s\n", line->text, line->sequence, line->quality);
	}

	inline void WriteAsFastALine(FILE * file) {
		fprintf(file, ">%s\t%u\t%u\n%s\n", text, index, 0U, sequence);
	}
};

class FastQFile {
	arg::cHugeArray<FastQLine> lines;

	inline void RTrim(char * line) {
		unsigned int length = strlen(line) - 1;
		while (line[length] == '\n' || line[length] == '\r') {
			line[length] = '\0';
			length--;
		}
	}

public:

	void LoadFromFile(char * filename) {
		lines.Clear();
		FILE * file = fopen(filename, "rt");

		FastQLine line;
		char temp[32];
		while (true) {
			if (fgets(line.text, 200, file) == NULL) {
				break;
			}
			RTrim(line.text);
			strcpy(line.text, line.text + 1); // remove @
			if (fgets(line.sequence, 200, file) == NULL) {
				break;
			}
			RTrim(line.sequence);
			fgets(temp, 31, file);
			if (temp[0] != '+') {
				throw "Invalid sequence";
			} // test +
			fgets(line.quality, 200, file);
			RTrim(line.quality);
			line.length = strlen(line.sequence);
			line.index = lines.Count() + 1;
			lines.Add(&line);
		}

		fclose(file);
	}

	void SaveAsFastAFile(char * filename) {
		FILE * fasta = fopen(filename, "w");
		for (unsigned i = 0; i < lines.Count(); i++) {
			lines[i].WriteAsFastALine(fasta);
		}
		fclose(fasta);
	}

	inline FastQLine * operator[](const unsigned int index) {
		return &lines[index];
	}

	inline unsigned int Count() {
		return lines.Count();
	}

};

class Roadmap {
public:
	unsigned int source; // current line ID
	unsigned int target; // target line ID
	unsigned int sourceoffset; // offset of kmer in current line from 0 inclusive
	unsigned int targetstart; // offset of kmer in target line from 0 inclusive
	unsigned int targetend; // number of kmers roadmap

	void Print() {
		printf("ROADMAP %u: %u\t%u\t%u\t%u\n", source + 1, target + 1, sourceoffset, targetstart, targetend);
	}
};

class KmerOccurence {
public:

	KmerOccurence(unsigned lineIndex, unsigned short writeNucleotideIndex) {
		this->lineIndex = lineIndex;
		this->writeNucleotideIndex = writeNucleotideIndex;
	}
	unsigned lineIndex;
	unsigned short writeNucleotideIndex;
};

class Roadmaps {
	arg::cHugeArray<Roadmap> roadmaps;
	unsigned kmerlength;

	struct KmerComparator {

		inline bool operator()(char const *a, char const *b) {
			return strncmp(a, b, KMER_LENGTH) < 0;
		}
	};

	std::map<char*, KmerOccurence*, KmerComparator> kmers;
	bool found;
	unsigned foundLength;
	unsigned writeNucleotideIndex;
	KmerOccurence * firstKmerOccurenceInLine;

	unsigned lineIndex, kmerIndex;

	inline void StoreRoadmap() {
		Roadmap roadmap;
		roadmap.source = lineIndex;
		roadmap.sourceoffset = writeNucleotideIndex;
		roadmap.target = firstKmerOccurenceInLine->lineIndex;
		roadmap.targetstart = firstKmerOccurenceInLine->writeNucleotideIndex;
		roadmap.targetend = firstKmerOccurenceInLine->writeNucleotideIndex + foundLength;
		roadmaps.Add(&roadmap);
		roadmap.Print();
	}

	inline void ProcessKmer(char * kmer) {
		found = kmers.find(kmer) != kmers.end();
		if (found) {
			foundLength++;
		} else {
			kmers[kmer] = new KmerOccurence(lineIndex, writeNucleotideIndex);
		}
		KmerOccurence * kmerOccurence = found ? kmers.find(kmer)->second : NULL;
		bool continuesOnLastFoundKmer = foundLength > 1 && found && firstKmerOccurenceInLine->lineIndex == kmerOccurence->lineIndex && firstKmerOccurenceInLine->writeNucleotideIndex + foundLength - 1 == kmerOccurence->writeNucleotideIndex;
		if (found && foundLength > 1 && !continuesOnLastFoundKmer) {
			StoreRoadmap();
			firstKmerOccurenceInLine = kmerOccurence;
			foundLength = 1;
		} else if (found && foundLength > 1 /* && continuesOnLastFoundKmer*/) {

		} else if (!found && foundLength > 0) {
			StoreRoadmap();
			++writeNucleotideIndex;
			firstKmerOccurenceInLine = NULL;
			foundLength = 0;
		} else if (found /* && foundLength <= 1*/) {
			firstKmerOccurenceInLine = kmerOccurence;
		} else /* if !found && foundLength <= 1*/ {
			++writeNucleotideIndex;
			firstKmerOccurenceInLine = NULL;
			foundLength = 0;
		}
	}

	inline void ProcessEndline() {
		if (found) {
			StoreRoadmap();
		}
		found = false;
		foundLength = 0;
		writeNucleotideIndex = 0;
		firstKmerOccurenceInLine = NULL;
		printf("\n");
	}

public:

	void LoadFromFastQFile(FastQFile * fastq, unsigned kmerlength) {
		this->kmerlength = kmerlength;
		roadmaps.Clear();
		kmers.clear();
		found = false;
		foundLength = 0;
		writeNucleotideIndex = 0;
		firstKmerOccurenceInLine = NULL;

		for (lineIndex = 0; lineIndex < fastq->Count(); lineIndex++) {
			FastQLine * line = (*fastq)[lineIndex];
			for (kmerIndex = 0; kmerIndex < line->length - kmerlength + 1; kmerIndex++) {
				ProcessKmer(&line->sequence[kmerIndex]);
			}
			ProcessEndline();
		}
	}

	void SaveRoadmaps(char * filename) {
		FILE * file = fopen(filename, "w");
		unsigned int lastIndex = 0;

		for (unsigned i = 0; i < roadmaps.Count(); i++) {
			if (roadmaps[i].source + 1 != lastIndex) {
				fprintf(file, "ROADMAP %u\n", roadmaps[i].source + 1);
				lastIndex = roadmaps[i].source + 1;
			}
			fprintf(file, "%u\t%u\t%u\t%u\n", roadmaps[i].target + 1, roadmaps[i].sourceoffset, roadmaps[i].targetstart, roadmaps[i].targetend);
		}
		fclose(file);
	}

};

int main(int argc, char** argv) {

	Stopwatch sw;

	try {
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/1/frag_2.fastq";
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/0/NN.fastq";
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/0/40lines.fastq";
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/1/400.fastq";
		char * sourcefilename = argv[1];


		arg::dna::cFastQFileLoader loader(sourcefilename);
		arg::dna::cKmerBuilder kmerBuilder(loader, 31);
		//		arg::dna::cKmerOccurenceMapRBTree map;
		//		arg::dna::cKmerOccurenceMapHashMap map;
		arg::dna::cKmerOccurenceMapUnorderedMap map;
		arg::dna::cRoadmapBuilder roadmapBuilder(kmerBuilder, map);

		roadmapBuilder.Build();

		loader.Close();


	} catch (char const * e) {
		printf("ERROR: %s\n", e);
	} catch (const char e) {
		printf("ERROR: '%c' (%d)\n", e, e);
	}

	sw.Stop().Display();
	return 0;

	//	char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/0/16lines.fastq";
	//	char * fastafilename = (char*) "/home/danrob/VSB/DNATests/0/Sequences.fa";
	//	char * roadmapsfilename = (char*) "/home/danrob/VSB/DNATests/0/Roadmaps.fa";
	//
	//	FastQFile fastq;
	//	fastq.LoadFromFile(sourcefilename);
	//	fastq.SaveAsFastAFile(fastafilename);
	//
	//	Roadmaps roadmaps;
	//	roadmaps.LoadFromFastQFile(&fastq, KMER_LENGTH);
	//	roadmaps.SaveRoadmaps(roadmapsfilename);

	//	k zamysleni: nalezne se vzdy nejdelsi shoda?

	return 0;
}

