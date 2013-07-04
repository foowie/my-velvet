/* 
 * File:   cDeBruijnGraph.h
 * Author: danrob
 *
 * Created on 17. červen 2013, 14:51
 */

#include <iostream>
#include "BaseTypes.h"
#include "cDeBruijnEdge.h"
#include "cDeBruijnNode.h"
#include "../core/cHugeArray.h"
#include <unordered_map>

#ifndef CDEBRUIJNGRAPH_H
#define	CDEBRUIJNGRAPH_H

namespace arg {
	namespace dna {

		class cDeBruijnGraph {
		private:
			unsigned short kmerLength;
			cHugeArray<cDeBruijnEdge> edges;
			cHugeArray<cDeBruijnNode> nodes;
			
			cDeBruijnNode tempNode;
			cDeBruijnEdge tempEdge;
			std::unordered_map<Kmer, cDeBruijnNode*, std::hash < Kmer >> * nodeMap;

		public:

			cDeBruijnGraph(unsigned short pKmerLength) : kmerLength(pKmerLength) {
				nodeMap = new std::unordered_map<Kmer, cDeBruijnNode*, std::hash < Kmer >> (1000);
			}
			
			void AddKmer(Kmer kmer) {
				cDeBruijnNode * node = (*nodeMap)[kmer];
				if(node != NULL) {
					node->quantity++;
				} else {
					tempNode.edgeCount = 0;
					tempNode.edges = NULL;
					tempNode.kmer = kmer;
					tempNode.quantity = 1;
					nodes.Add(&tempNode);
					(*nodeMap)[kmer] = &nodes[nodes.Count() - 1];
				}
			}
			
			void CreateEdges() {
				Kmer clear = 0x0ULL;
				for(unsigned int i = 0; i < kmerLength; i++) {
					clear = (clear << 2) | 0x3ULL;
				}
				
				for(unsigned int i = 0; i < nodes.Count(); i++) {
					cDeBruijnNode * node = &nodes[i];
					for(unsigned long long int base = 0; base < 0x4ULL; base++) {
						Kmer destinationKmer = ((node->kmer << 2) | base) & clear;
						cDeBruijnNode * destinationNode = (*nodeMap)[destinationKmer];
						if(destinationNode != NULL) {
							if(node->GetEdgeToNode(destinationKmer) == NULL) {
								tempEdge.destination = destinationNode;
								tempEdge.next = NULL;
								edges.Add(&tempEdge);
								node->AddEdge(&edges[edges.Count() - 1]);
								node->edgeCount++;
							}
						}
					}
				}
			}
			
			void DisplayNodes() {
				for(unsigned int i = 0; i < nodes.Count(); i++) {
					cDeBruijnNode node = nodes[i];
					node.Display(kmerLength);
				}
			}
			
			void DisplayEdges() {
				for(unsigned int i = 0; i < nodes.Count(); i++) {
					cDeBruijnNode node = nodes[i];
					node.DisplayEdges(kmerLength);
				}
			}
			
		};

	}
}

#endif	/* CDEBRUIJNGRAPH_H */

