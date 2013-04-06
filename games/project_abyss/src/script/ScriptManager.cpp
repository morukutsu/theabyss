// abyss 
// by morukutsu (morukutsu@hotmail.com)

#include "ScriptManager.h"
#include "scriptstdstring.h"

// Implement a simple message callback function
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

// Print the script string to the standard output stream
void print(std::string &msg)
{
	printf("%s", msg.c_str());
}

ScriptManager::ScriptManager()
{
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	RegisterStdString(engine);

	engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); 

	// http://www.angelcode.com/angelscript/sdk/docs/manual/doc_hello_world.html
	builder.StartNewModule(engine, "MainModule"); 
}

ScriptManager::~ScriptManager()
{
	engine->Release();
}

void ScriptManager::LoadScript(std::string filename) 
{
	builder.AddSectionFromFile(filename.c_str() );
	int r = builder.BuildModule();
	if( r < 0 )
	{
	  // An error occurred. Instruct the script writer to fix the 
	  // compilation errors that were listed in the output stream.
	  printf("Please correct the errors in the script and try again.\n");
	  return;
	}
}

void ScriptManager::ExecuteScriptFunction(std::string decl)
{
	// Find the function that is to be called. 
	asIScriptModule *mod = engine->GetModule("MainModule");
	asIScriptFunction *func = mod->GetFunctionByDecl(decl.c_str() );
	if( func == 0 )
	{
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		printf("The script must have the function '%s'. Please add it and try again.\n", decl.c_str() );
		return;
	}

	// Create our context, prepare it, and then execute
	asIScriptContext *ctx = engine->CreateContext();
	ctx->Prepare(func);
	int r = ctx->Execute();
	if( r != asEXECUTION_FINISHED )
	{
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION ) 
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
		}
	}
}