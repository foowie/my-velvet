/* 
 * File:   cDeBruijnGraphBuilder.h
 * Author: danrob
 *
 * Created on 17. červen 2013, 16:17
 */

#ifndef CDEBRUIJNGRAPHBUILDER_H
#define	CDEBRUIJNGRAPHBUILDER_H

#include "cDeBruijnGraph.h"
#include "cKmerBuilder.h"


namespace arg {
	namespace dna {

		class cDeBruijnGraphBuilder {
		private:
			cDeBruijnGraph * graph;
			cKmerBuilder * kmerBuilder;
		public:

			cDeBruijnGraphBuilder(cDeBruijnGraph * pGraph, cKmerBuilder * pKmerBuilder)
			: graph(pGraph), kmerBuilder(pKmerBuilder) {
			}
			
			void Build() {
				while(kmerBuilder->Next()) {
					graph->AddKmer(kmerBuilder->GetKmer());
				}
			}
		};

	}
}

#endif	/* CDEBRUIJNGRAPHBUILDER_H */

