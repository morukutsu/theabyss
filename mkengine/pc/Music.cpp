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
		bgm.setLoop(false);
	}

	void Music::Stop()
	{
		bgm.stop();
	}

	void Music::SetLoop(bool value)
	{
		bgm.setLoop(value);
	}
};