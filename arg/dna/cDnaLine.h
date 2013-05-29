/* 
 * File:   cDnaLine.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:08
 */

#ifndef CDNALINE_H
#define	CDNALINE_H

namespace arg {
	namespace dna {

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
	}
}

#endif	/* CDNALINE_H */

