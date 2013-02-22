// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_ASCIIFILE
#define MK_ASCIIFILE

#include "Ressource.h"
#include <string>

// Ascii Text file ressource
namespace mk 
{
	class AsciiFile : public Ressource
	{
		public:
			AsciiFile();
			~AsciiFile();
			int LoadFromFile(const char* filename);		// Loads a png image from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The file pointer must be positionned on the png image
			int LoadFromMemory(char* mem, int size);	// Load a png from memory
			void Free();

			const char *getString();
			int getSize();

		public:
			char* buffer;
			int filesize;
	};
};

#endif