// mkengine 
// by morukutsu (morukutsu@hotmail.com)

// Classe permettant d'appliquer des Pixel Shaders

#ifndef MK_SHADER
#define MK_SHADER

#include <string>
#include <SFML/Graphics.hpp>
#include "Ressource.h"

namespace mk 
{
	class Shader : public Ressource
	{
		public:
			Shader();
			~Shader();

			int LoadFromFile(const char* filename);	
			int LoadFromFile(FILE* fp, long size);		
			int LoadFromMemory(char* mem, int size);

			void Load(std::string text, bool vertex = false);
			void Bind();
			void Unbind();

		public:
			sf::Shader shader;

			char* buffer;
			int filesize;
	};
};

#endif