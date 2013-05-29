/* 
 * File:   cLoader.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:08
 */

#include "cDnaLine.h"

#ifndef CLOADER_H
#define	CLOADER_H

namespace arg {
	namespace dna {

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
	}
}

#endif	/* CLOADER_H */

