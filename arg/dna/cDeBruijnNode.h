/* 
 * File:   cDeBruijnNode.h
 * Author: danrob
 *
 * Created on 17. červen 2013, 14:51
 */

#include "BaseTypes.h"
#include "cDeBruijnEdge.h"

#ifndef CDEBRUIJNNODE_H
#define	CDEBRUIJNNODE_H

namespace arg {
	namespace dna {

		class cDeBruijnNode {
		public:
			Kmer kmer;
			cDeBruijnEdge * edges;
			unsigned short int quantity;
			unsigned short int edgeCount;
			
			cDeBruijnEdge * GetEdgeToNode(const Kmer kmer) {
				cDeBruijnEdge * edge = edges;
				while(edge != NULL) {
					if(edge->destination->kmer == kmer) {
						return edge;
					}
					edge = edge->next;
				}
				return NULL;
			}
			
			void AddEdge(cDeBruijnEdge * newEdge) {
				if(edges == NULL) {
					edges = newEdge;
				} else {
					cDeBruijnEdge * edge = edges;
					while(edge->next != NULL) {
						edge = edge->next;
					}
					edge->next = newEdge;
				}
			}
			
			void Display(const unsigned short length) {
				DisplayKmer(length);
				printf(" (%llu) Q=%d E=%d\n", kmer, quantity, edgeCount);
			}
			
			void DisplayKmer(const unsigned short length) {
				for(unsigned short i = 1; i <= length; i++) {
					Kmer c = (kmer >> ((length - i) * 2)) & 0x3ULL;
					switch(c) {
						case BASE_A: putchar('A'); break;
						case BASE_C: putchar('C'); break;
						case BASE_G: putchar('G'); break;
						case BASE_T: putchar('T'); break;
						default: putchar('?'); break;
					}
				}
			}
			
			void DisplayEdges(const unsigned short length) {
				cDeBruijnEdge * edge = edges;
				if(edge != NULL) {
					DisplayKmer(length);
					printf(" -> ");
					while(edge != NULL) {
						edge->destination->DisplayKmer(length);
						printf(" ");
						edge = edge->next;
					}				
					printf("\n");
				}
			}
			
		};
	}
}

#endif	/* CDEBRUIJNNODE_H */

