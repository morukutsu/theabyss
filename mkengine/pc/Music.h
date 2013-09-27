// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_MUSIC
#define H_MUSIC

#include "mkengine.h"

namespace mk
{
	class Music
	{
		public:
			Music();
			~Music();

			void Open(std::string filename);
			void Play();
			void Stop();

		public:
			sf::Music bgm;
			RessourceFileStream stream;
	};
};

#endif