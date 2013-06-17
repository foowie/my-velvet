#include <stdio.h>
#include "Stopwatch.h"
#include "Memwatch.h"

#include "arg/dna/cFastQFileLoader.h"
#include "arg/dna/cFastAFileWriter.h"
#include "arg/dna/cKmerBuilder.h"
#include "arg/dna/cKmerOccurenceMapUnorderedMap.h"
#include "arg/dna/cRoadmapBuilder.h"

namespace arg {
	namespace dna {
	}
}

int main(int argc, char** argv) {

	Stopwatch sw;

	try {
		unsigned short kmerLength = 31;
//				char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/1/frag_2.fastq";
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/0/NN.fastq";
		//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/0/40lines.fastq";
//		char * sourcefilename = (char*) "/home/danrob/VSB/DNATests/1/400.fastq";
		char * fastafilename = (char*) "/home/danrob/VSB/DNATests/FASTA.fasta";
		char * sourcefilename = argv[1];


		arg::dna::cFastQFileLoader loader(sourcefilename);
		arg::dna::cFastAFileWriter writer(loader);
		writer.SaveToFile(fastafilename);
		printf("%u\t%u\t%u\t%u\n", writer.GetRecordCount(), 0U, kmerLength, 0U);
		
		arg::dna::cKmerBuilder kmerBuilder(loader, kmerLength);
//		arg::dna::cKmerOccurenceMapRBTree map;
		arg::dna::cKmerOccurenceMapUnorderedMap map;
		arg::dna::cRoadmapBuilder roadmapBuilder(kmerBuilder, map);

		roadmapBuilder.Build();

		loader.Close();

		Memwatch mem(false);
		mem.DisplayPeak();

	} catch (char const * e) {
		printf("ERROR: %s\n", e);
	} catch (const char e) {
		printf("ERROR: '%c' (%d)\n", e, e);
	}

	sw.Stop().Display();
	return 0;
}

