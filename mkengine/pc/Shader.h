// mkengine 
// by morukutsu (morukutsu@hotmail.com)

// Classe permettant d'appliquer des Pixel Shaders

#ifndef MK_SHADER
#define MK_SHADER

#include <string>
#include <SFML/Graphics.hpp>

namespace mk 
{
	class Shader 
	{
		public:
			Shader();
			~Shader();

			void Load(std::string text);
			void Bind();
			void Unbind();

		public:
			sf::Shader shader;
	};
};

#endif