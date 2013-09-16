// mkengine
// LittleSpace Studio 2012

#include "ParticleGeneratorRessource.h"

namespace mk 
{
	RangeFunc* createFunc(std::string name)
	{
		if(name == "sin")
			return new RangeFuncSin();
		else if(name == "random")
			return new RangeFuncRandom();
		else
			return NULL;
	}

	ParticleGeneratorRessource::ParticleGeneratorRessource()
	{
		// Intialisation
		maxParticles = -1;
		initialLife = 1.0f;
		initialSize = 1.0f;
		gravity = 0.0f;
		initialAngle = 0.0f;
		isAreaGenerator = false;
		areaZ_min = areaZ_max = 0.0f;
		limitParticles = 0;				// Unlimited
		emitAmount = 1;					// 1 particle per tick

		// Initialisations range funcs
		for(int i = 0; i < MAX_PARAMETERS; i++)
			rangeFuncs[i] = NULL;
	}

	ParticleGeneratorRessource::~ParticleGeneratorRessource()
	{
		for(int i = 0; i < MAX_PARAMETERS; i++)
		{
			if(rangeFuncs[i])
				delete rangeFuncs[i];
		}
	}

	void ParticleGeneratorRessource::Load(const char* buffer)
	{
		TiXmlDocument doc;
		doc.Parse(buffer);

		TiXmlElement * root = doc.FirstChildElement("Generator");

		TiXmlElement * elem = root->FirstChildElement("Parameter");
		while(elem)
		{
			std::string parameterName = elem->Attribute("name");

			if(parameterName == "Sprite")
			{
				TiXmlElement * elem2 = elem->FirstChildElement("Sprite");
				while(elem2)
				{
					std::string spriteFilename = elem2->Attribute("filename");
					GenSprite genSpr;
				
					genSpr.img = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("particles/" + spriteFilename);
					elem2->QueryFloatAttribute("frequency", &genSpr.frequency);

					imgs.push_back(genSpr);

			
					elem2 = elem2->NextSiblingElement();
				}	
			} 
			else if(parameterName == "MaxParticles")
			{
				elem->QueryIntAttribute("value", &maxParticles);
			}
			else if(parameterName == "InitialLife")
			{
				elem->QueryFloatAttribute("value", &initialLife);
			}
			else if(parameterName == "InitialSize")
			{
				elem->QueryFloatAttribute("value", &initialSize);
			}
			else if(parameterName == "SizeVariation")
			{
				elem->QueryFloatAttribute("value", &sizeVariation);
			}
			else if(parameterName == "Frequency")
			{
				elem->QueryFloatAttribute("value", &frequency);
			}
			else if(parameterName == "Gravity")
			{
				elem->QueryFloatAttribute("value", &gravity);
			}
			else if(parameterName == "Fade")
			{
				elem->QueryFloatAttribute("value", &fade);
			}
			else if(parameterName == "FadeRange_Min")
			{
				elem->QueryFloatAttribute("value", &fade_rmin);
			}
			else if(parameterName == "FadeRange_Max")
			{
				elem->QueryFloatAttribute("value", &fade_rmax);
			}
			else if(parameterName == "Velocity_X")
			{
				elem->QueryFloatAttribute("value", &vx);
			}
			else if(parameterName == "Velocity_Y")
			{
				elem->QueryFloatAttribute("value", &vy);
			}
			else if(parameterName == "Velocity_Z")
			{
				elem->QueryFloatAttribute("value", &vz);
			}
			else if(parameterName == "Velocity_X_Range_Min")
			{
				elem->QueryFloatAttribute("value", &vx_rmin);
			}
			else if(parameterName == "Velocity_Y_Range_Min")
			{
				elem->QueryFloatAttribute("value", &vy_rmin);
			}
			else if(parameterName == "Velocity_Z_Range_Min")
			{
				elem->QueryFloatAttribute("value", &vz_rmin);
			}
			else if(parameterName == "Velocity_X_Range_Max")
			{
				elem->QueryFloatAttribute("value", &vx_rmax);
			}
			else if(parameterName == "Velocity_Y_Range_Max")
			{
				elem->QueryFloatAttribute("value", &vy_rmax);
			}
			else if(parameterName == "Velocity_Z_Range_Max")
			{
				elem->QueryFloatAttribute("value", &vz_rmax);
			}
			else if(parameterName == "InitialAlpha")
			{
				elem->QueryFloatAttribute("value", &initialAlpha);
			}
			else if(parameterName == "AlphaFade")
			{
				elem->QueryFloatAttribute("value", &alphaFade);
			}
			else if(parameterName == "InitialAngle")
			{
				elem->QueryFloatAttribute("value", &initialAngle);
			}
			else if(parameterName == "AngleVariation")
			{
				elem->QueryFloatAttribute("value", &angleVariation);
			}
			else if(parameterName == "InitialColor_Red")
			{
				int v;
				elem->QueryIntAttribute("value", &v);
				initialColorR = v/255.0f;
			}
			else if(parameterName == "InitialColor_Green")
			{
				int v;
				elem->QueryIntAttribute("value", &v);
				initialColorG = v/255.0f;
			}
			else if(parameterName == "InitialColor_Blue")
			{
				int v;
				elem->QueryIntAttribute("value", &v);
				initialColorB = v/255.0f;
			}
			else if(parameterName == "Func_Velocity_X")
			{
				rangeFuncs[PARAM_VEL_X] = createFunc(elem->Attribute("value"));
			}
			else if(parameterName == "Func_Velocity_Y")
			{
				rangeFuncs[PARAM_VEL_Y] = createFunc(elem->Attribute("value"));
			}
			else if(parameterName == "Func_Velocity_Z")
			{
				rangeFuncs[PARAM_VEL_Z] = createFunc(elem->Attribute("value"));
			}
			else if(parameterName == "Referencial")
			{
				std::string value = elem->Attribute("value");
				if(value == "local")
				{
					referencial = false;
				}
				else if(value == "world")
				{
					referencial = true;
				}
			}
			else if(parameterName == "Color_Keyframes")
			{
				TiXmlElement* elem2 = elem->FirstChildElement("ColorKey");
				// Loading each Colorkey
				while(elem2)
				{
					ColorKey key;
					elem2->QueryFloatAttribute("time", &key.time);

					int v;
					elem2->QueryIntAttribute("R", &v);
					key.r = v/255.0f;
					elem2->QueryIntAttribute("G", &v);
					key.g = v/255.0f;
					elem2->QueryIntAttribute("B", &v);
					key.b = v/255.0f;

					colorKeys.push_back(key);
					elem2 = elem2->NextSiblingElement();
				}
			}
			else if(parameterName == "Area_Generator")
			{
				std::string value = elem->Attribute("value");
				if(value == "true")
					isAreaGenerator = true;
				else
					isAreaGenerator = false;
			}
			else if(parameterName == "Area_Type")
			{
				std::string value = elem->Attribute("value");
				if(value == "rect")
					areaType = AREA_RECT;
			}
			else if(parameterName == "Area_Z_Min")
			{
				elem->QueryFloatAttribute("value", &areaZ_min);
			}
			else if(parameterName == "Area_Z_Max")
			{
				elem->QueryFloatAttribute("value", &areaZ_max);
			}
			else if(parameterName == "LimitParticles")
			{
				int v;
				elem->QueryIntAttribute("value", &v);
				limitParticles = v;
			}
			else if(parameterName == "EmitAmount")
			{
				int v;
				elem->QueryIntAttribute("value", &v);
				emitAmount = v;
			}
			elem = elem->NextSiblingElement();
		}
	}

	int ParticleGeneratorRessource::LoadFromFile(const char* filename)
	{
		// Chargement du fichier en mémoire
		FILE* fp = fopen(filename, "rb");
		if(fp)
		{
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = (char*)malloc(filesize + 1);
			fread(buffer, sizeof(char), filesize, fp);
			buffer[filesize] = '\0';

			// Chargement XML
			Load(buffer);

			// Libération
			free(buffer);

			mLoaded = true;
			return 0;
		}
		else
		{
			return 1;
		}	
	}

	int ParticleGeneratorRessource::LoadFromFile(FILE* fp, long size)
	{
		// Chargement du fichier en mémoire
		if(fp)
		{
			int filesize = size;

			char* buffer = (char*)malloc(filesize + 1);
			fread(buffer, sizeof(char), filesize, fp);
			buffer[filesize] = '\0';

			// Chargement XML
			Load(buffer);

			// Libération
			free(buffer);

			mLoaded = true;
			return 0;
		}
		else
		{
			return 1;
		}	
	}

	int ParticleGeneratorRessource::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}
};