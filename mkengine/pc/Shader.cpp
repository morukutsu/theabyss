// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Shader.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>


namespace mk 
{
	Shader::Shader()
	{
		
	}

	Shader::~Shader()
	{

	}

	void Shader::Load(std::string text)
	{
		shader.loadFromMemory(text, sf::Shader::Type::Fragment);
	}

	void Shader::Bind()
	{
		sf::Shader::bind(&shader);
	}

	void Shader::Unbind()
	{
		sf::Shader::bind(NULL);
	}
};