// abyss 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_SCRIPT_MANAGER
#define H_SCRIPT_MANAGER

#include "singleton.h"

#include <string>

#include <angelscript.h>
#include "scriptbuilder.h"
#include "scriptarray.h"
#include "scriptmath.h"

class ScriptManager : public Singleton<ScriptManager>
{
	friend class Singleton<ScriptManager>;

	public:
		ScriptManager();
		~ScriptManager();

		// Loading scripts
		void Init();
		void LoadScript(std::string filename);
		void Build();
		void Clean();

		void ExecuteScriptFunction(std::string decl);
		
		asIScriptEngine* GetEngine() { return engine; }

	private:
		asIScriptEngine *engine;
		CScriptBuilder builder;

};

#endif