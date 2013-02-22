// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Sound.h"
#include <iostream>

// Image : platform specific image handling
namespace mk 
{
	Sound::Sound()
	{
		mLoadedFrom = mUsedMemory = -1;
	}

	Sound::~Sound()
	{
	}

	int Sound::LoadFromFile(const char* filename)
	{
		if(Buffer.loadFromFile(filename) )
		{
			mLoaded = true;
			return 0;
		}
		else
		{
			return -1;
		}
	}

	int Sound::LoadFromFile(FILE* fp, long size)
	{
		//Sur PC, on charge le fichier entièrement en mémoire
		char* buffer = (char*)malloc(size);
		fread(buffer, sizeof(char), size, fp);
		
		//Chargement du son
		Buffer.loadFromSamples((sf::Int16*)buffer, size/2, 2, 44100);
		//Buffer.LoadFromMemory(buffe, size);

		free(buffer);
		mLoaded = true;
		return 0;
	}

	int Sound::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	void Sound::Free()
	{
		
	}

	int Sound::getLoadSource()
	{
		return mLoadedFrom;
	}

	int Sound::getUsedMemory()
	{
		return mUsedMemory;
	}
};