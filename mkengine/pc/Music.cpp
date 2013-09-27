// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Music.h"

namespace mk
{
	Music::Music()
	{

	}

	Music::~Music()
	{

	}

	void Music::Open(std::string filename)
	{
		stream.open(filename);
		bgm.openFromStream(stream);
	}

	void Music::Play()
	{
		bgm.play();
		bgm.setLoop(true);
	}

	void Music::Stop()
	{
		bgm.stop();
	}
};