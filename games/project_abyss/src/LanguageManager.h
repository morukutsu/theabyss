// Natura 2
// by morukutsu (morukutsu@hotmail.com)

//Includes
#ifndef H_LANGUAGEMANAGER
#define H_LANGUAGEMANAGER

#include "mkengine.h"
#include <vector>
#include <string>
#include "loc_header.h"

class LanguageManager
{
	enum { LANG_FR, LANG_EN };

	public:
		LanguageManager();
		~LanguageManager();

		static void SetLanguage(int id);
		static void LoadLanguage(mk::AsciiFile* file);
		static std::string& GetString(int txt);
		static const char* GetCString(int txt);

	private:
		static int curLang;
		static std::vector<std::string> mLanguageMap;
		static std::string nullStr;
};

#endif