// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_SOUND
#define MK_SOUND

#include <SFML/Audio.hpp>
#include "Ressource.h"
#include "gctypes.h"

// Sound : platform specific image handling
namespace mk 
{
	enum { SND_FROM_FILE, SND_FROM_RESSOURCE_MANAGER };

	class Sound : public Ressource
	{
		public:
			Sound();
			~Sound();
			int LoadFromFile(const char* filename);		// Loads a sound from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The file pointer must be positionned on the sound
			int LoadFromMemory(char* mem, int size);	// Load a sound from memory
			void Free();

			int getLoadSource();
			int getUsedMemory();

		public:
			int mLoadedFrom, mUsedMemory;

			//Platform specific data
			sf::SoundBuffer Buffer;
	};
};

#endif