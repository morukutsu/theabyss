// abyss 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_SCRIPT_MANAGER
#define H_SCRIPT_MANAGER

#include "singleton.h"

#include <string>

#include <angelscript.h>
#include "scriptbuilder.h"

class ScriptManager : public Singleton<ScriptManager>
{
	friend class Singleton<ScriptManager>;

	public:
		ScriptManager();
		~ScriptManager();

		// Loading scripts
		void LoadScript(std::string filename);
		void ExecuteScriptFunction(std::string decl);

	private:
		asIScriptEngine *engine;
		CScriptBuilder builder;

};

#endif