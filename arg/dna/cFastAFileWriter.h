/* 
 * File:   cFastaFileWriter.h
 * Author: Daniel Robenek
 *
 * Created on 29. květen 2013, 22:44
 */

#ifndef CFASTAFILEWRITER_H
#define	CFASTAFILEWRITER_H

namespace arg {
	namespace dna {

		class cFastAFileWriter {
		private:
			cLoader * loader;
			unsigned int records;
			unsigned long long int bases;
		public:

			cFastAFileWriter(cLoader & pLoader) : loader(&pLoader) {
				records = 0;
				bases = 0;
			}

			void SaveToFile(char * fileName) {
				FILE * file = fopen(fileName, "w");
				arg::dna::cDnaLine * line;
				if (file == NULL) {
					throw "Can't write to file!";
				}
				loader->Reset();
				while ((line = loader->ReadLine()) != NULL) {
					fprintf(file, ">%s\n%s\n", line->text, line->sequence);
					records++;
					bases += line->length;
				}
				fclose(file);
			}

			unsigned int GetRecordCount() {
				return records;
			}

			unsigned long long int GetBaseCount() {
				return bases;
			}
		};
	}
}

#endif	/* CFASTAFILEWRITER_H */

