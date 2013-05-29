/* 
 * File:   cFastQFileLoader.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:09
 */

#include <iostream>
#include "cLoader.h"
#include "cDnaLine.h"

#ifndef CFASTQFILELOADER_H
#define	CFASTQFILELOADER_H

namespace arg {
	namespace dna {

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
				if(fgets(temp, 65534, file) == NULL || temp[0] != '+') {
					throw "Invalid FastQ sequence";
				}
				if(fgets(temp, 64435, file) == NULL) {
					throw "Invalid FastQ sequence";
				}
				currentLine->index++;
				return currentLine;
			}

			void Reset() {
				rewind(file);
			}

			void Close() {
				if (file != NULL) {
					fclose(file);
					file = NULL;
				}
			}

		};
	}
}

#endif	/* CFASTQFILELOADER_H */

