// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_RESSOURCE
#define MK_RESSOURCE

#include <stdio.h>

namespace mk
{
	class Ressource
	{
	public:
		Ressource()
		{
			mLoaded = false;
			mFromRessourceManager = false;
		}

		virtual ~Ressource() { } 

	public:
		virtual int LoadFromFile(const char* filename) = 0;	
		virtual int LoadFromFile(FILE* fp, long size) = 0;		
		virtual int LoadFromMemory(char* mem, int size) = 0;

		bool mLoaded;
		bool mFromRessourceManager;
	};
};

#endif