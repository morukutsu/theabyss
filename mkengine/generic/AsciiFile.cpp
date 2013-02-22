// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "AsciiFile.h"
#include <stdlib.h>
#include <iostream>

// Ascii Text file ressource
namespace mk 
{
	AsciiFile::AsciiFile()
	{
		filesize = 0;
	}

	AsciiFile::~AsciiFile()
	{
		free(buffer);
		filesize = 0;
		mLoaded = false;

		std::cout << "ascii file destroyed" << std::endl;
	}

	int AsciiFile::LoadFromFile(const char* filename)
	{
		//On charge le fichier entièrement en mémoire
		FILE* fp = fopen(filename, "rb");
		if(fp)
		{
			fseek(fp, 0, SEEK_END);
			filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			buffer = (char*)malloc(filesize);
			fread(buffer, sizeof(char), filesize, fp);
		
			fclose(fp);
			mLoaded = true;

			return 0;
		}
		else
		{
			return 1;
		}	
	}	

	int AsciiFile::LoadFromFile(FILE* f, long size)
	{
		//On charge le fichier entièrement en mémoire
		FILE* fp = f;
		if(fp)
		{
			filesize = size;

			buffer = (char*)malloc(filesize);
			fread(buffer, sizeof(char), filesize, fp);
		
			mLoaded = true;
			return 0;
		}
		else
		{
			return 1;
		}	
	}	

	int AsciiFile::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	void AsciiFile::Free()
	{
		free(buffer);
		filesize = 0;
		mLoaded = false;
	}

	const char* AsciiFile::getString()
	{
		return (const char*)buffer;
	}

	int AsciiFile::getSize()
	{
		return filesize;
	}
};