/* 
 * File:   cDeBruijnNode.h
 * Author: danrob
 *
 * Created on 17. červen 2013, 14:51
 */

#include "BaseTypes.h"
//#include "cDeBruijnNode.h"

#ifndef CDEBRUIJNEDGE_H
#define	CDEBRUIJNEDGE_H


namespace arg {
	namespace dna {

		class cDeBruijnNode;
		class cDeBruijnEdge {
		public:
			cDeBruijnNode * destination;
			cDeBruijnEdge * next;
		};
	}
}

#endif	/* CDEBRUIJNEDGE_H */

