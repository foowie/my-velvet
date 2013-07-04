/* 
 * File:   cStringLineLoader.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:09
 */

#include <iostream>
#include "cLoader.h"
#include "cDnaLine.h"

#ifndef CSTRINGLINELOADER_H
#define	CSTRINGLINELOADER_H

namespace arg {
	namespace dna {

		class cStringLineLoader : public cLoader {
		protected:
			const char * lines;
			cDnaLine * currentLine;
			unsigned int position;
		public:

			cStringLineLoader(const char * pLines) : lines(pLines) {
				currentLine = new cDnaLine(65534, 65534);
				position = 0;
			}

			~cStringLineLoader() {
				delete currentLine;
			}

			cDnaLine * ReadLine() {
				if(lines[position] == '\0') {
					return NULL;
				}
				currentLine->length = 0;
				currentLine->text[0] = 0;
				while(lines[position] != '\n' && lines[position] != ' ' && lines[position] != '\0') {
					currentLine->sequence[currentLine->length++] = lines[position++];
				}
				if(lines[position] != '\0') {
					position++;
				}
				currentLine->index++;
				return currentLine;
			}

			void Reset() {
				position = 0;
				currentLine->index = 0;
			}

			void Close() {
			}

		};
	}
}

#endif	/* CSTRINGLINELOADER_H */

