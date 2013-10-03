// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Shader.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "AsciiFile.h"
#include <iostream>

namespace mk 
{
	Shader::Shader()
	{
		filesize = 0;
	}

	Shader::~Shader()
	{
		free(buffer);
		filesize = 0;
		mLoaded = false;

		std::cout << "shader unloaded" << std::endl;
	}

	void Shader::Load(std::string text, bool vertex)
	{
		std::cout << "Compiling shader..." << std::endl;
		if(!vertex)
		{
			if(shader.loadFromMemory(text, sf::Shader::Type::Fragment) )
				mLoaded = true;
		}
		else
		{
			if(shader.loadFromMemory(text, sf::Shader::Type::Vertex) )
				mLoaded = true;
		}
	}

	void Shader::Bind()
	{
		sf::Shader::bind(&shader);
	}

	void Shader::Unbind()
	{
		sf::Shader::bind(NULL);
	}

	int Shader::LoadFromFile(const char* filename)
	{
		FILE* fp = fopen(filename, "rb");
		if(fp)
		{
			fseek(fp, 0, SEEK_END);
			filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			buffer = (char*)malloc(filesize + 1);
			fread(buffer, sizeof(char), filesize, fp);
			buffer[filesize] = '\0';

			fclose(fp);

			Load(buffer);

			return 0;
		}
		else
		{
			return 1;
		}	
	}

	int Shader::LoadFromFile(FILE* f, long size)
	{
		//On charge le fichier entièrement en mémoire
		FILE* fp = f;
		if(fp)
		{
			filesize = size;

			buffer = (char*)malloc(filesize + 1);
			fread(buffer, sizeof(char), filesize, fp);
			buffer[filesize] = '\0';

			Load(buffer);

			return 0;
		}
		else
		{
			return 1;
		}	
	}

	int Shader::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}
};