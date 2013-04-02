// abyss 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_SCRIPT_MANAGER
#define H_SCRIPT_MANAGER

#include "singleton.h"

#include <string>

#include <angelscript.h>

class ScriptManager : public Singleton<ScriptManager>
{
	friend class Singleton<ScriptManager>;

	public:
		ScriptManager();

	private:
		asIScriptEngine *engine;
		

};

#endif