// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "ExtendedSpriteRessource.h"
#include "RessourceManager.h"

namespace mk {
	ExtendedSpriteRessource::ExtendedSpriteRessource()
	{

	}

	ExtendedSpriteRessource::~ExtendedSpriteRessource()
	{
		// Freeing all the asociated bitmaps
		for(std::map<std::string, DOMBitmap>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); it++)
		{
			RessourceManager::getInstance()->DeleteRessource((it)->second.bitmap);
		}
	}

	// Loads an ExtendedSpriteRessource from file (with filename)
	int ExtendedSpriteRessource::LoadFromFile(const char* filename)
	{
		// Chargement de la ressource dans un AsciiFile
		AsciiFile f;
		if(f.LoadFromFile(filename) == 1)
			return 1;

		// Parsing du fichier XML
		Parse(f);

		return 0;
	}

	// The ExtendedSpriteRessource pointer must be positionned on the ExtendedSpriteRessource
	int ExtendedSpriteRessource::LoadFromFile(FILE* fp, long size)
	{
		// Chargement de la ressource dans un AsciiFile
		AsciiFile f;
		if(f.LoadFromFile(fp, size) == 1)
			return 1;

		// Parsing du fichier XML
		Parse(f);

		return 0;
	}

	// Load an ExtendedSpriteRessource from memory
	int ExtendedSpriteRessource::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	// Parsing du fichier XML
	void ExtendedSpriteRessource::Parse(AsciiFile& f)
	{
		// Ouverture du fichier XML
		TiXmlDocument doc;
		doc.Parse(f.getString() );
		TiXmlElement * root = doc.FirstChildElement("ExtendedSprite");

		// Parsing des bitmaps
		TiXmlElement * elem = root->FirstChildElement("Bitmaps")->FirstChildElement("Bitmap");
		while(elem)
		{
			// Appel au ressource manager, TODO : gérer le préfixe de chargement
			DOMBitmap bitmap;
			bitmap.path = elem->Attribute("path");
			bitmap.name = elem->Attribute("ressource_name");
			bitmap.bitmap = (Image*)mk::RessourceManager::getInstance()->LoadRessource(bitmap.path);
			bitmap.node_type = DOM_INSTANCE_TYPE_BITMAP;
			mBitmaps[bitmap.name] = bitmap;

			std::cout << bitmap.name << std::endl;

			elem = elem->NextSiblingElement();
		}

		// Parsing des models
		elem = root->FirstChildElement("Models")->FirstChildElement();
		while(elem)
		{
			DOMModel mod;
			mod.name = elem->Attribute("name");
			mod.node_type = DOM_INSTANCE_TYPE_MODEL;

			TiXmlElement * layerElem = elem->FirstChildElement("Layers")->FirstChildElement("Layer");
			while(layerElem)
			{
				// Création d'un layer
				DOMLayer lay;
				
				// Chargement des instances
				TiXmlElement * instElem = layerElem->FirstChildElement("Instance");
				while(instElem)
				{
					DOMInstance inst;

					if(strcmp(instElem->Attribute("type"), "bitmap") == 0)
						inst.instance_type = DOM_INSTANCE_TYPE_BITMAP;
					else if(strcmp(instElem->Attribute("type"), "model") == 0)
						inst.instance_type = DOM_INSTANCE_TYPE_MODEL;
					
					inst.of_name = instElem->Attribute("of");

					// Chargement du centre et matrice
					TiXmlElement * center = instElem->FirstChildElement("Center");
					center->QueryFloatAttribute("x", &inst.centerX);
					center->QueryFloatAttribute("y", &inst.centerY);

					TiXmlElement * mtx = instElem->FirstChildElement("Matrix");
					mtx->QueryFloatAttribute("a", &inst.a);
					mtx->QueryFloatAttribute("b", &inst.b);
					mtx->QueryFloatAttribute("c", &inst.c);
					mtx->QueryFloatAttribute("d", &inst.d);
					mtx->QueryFloatAttribute("tx", &inst.tx);
					mtx->QueryFloatAttribute("ty", &inst.ty);

					// Chargement du motion s'il existe
					TiXmlElement * motion = instElem->FirstChildElement("Motion");
					inst.hasMotionObject = false;
					if(motion)
					{
						inst.hasMotionObject = true;
						motion->Attribute("fps", &inst.fps);
						motion->Attribute("duration", &inst.duration);
						motion->Attribute("animStart", &inst.animStart);

						inst.fps /= 1000;
						inst.duration /= 1000;

						TiXmlElement * keyframe = motion->FirstChildElement("Keyframe");
						while(keyframe)
						{
							Keyframe key;

							keyframe->Attribute("time", &key.time);
							key.time /= 1000;

							keyframe->QueryFloatAttribute("a", &key.mtx.a);
							keyframe->QueryFloatAttribute("b", &key.mtx.b);
							keyframe->QueryFloatAttribute("c", &key.mtx.c);
							keyframe->QueryFloatAttribute("d", &key.mtx.d);
							keyframe->QueryFloatAttribute("tx", &key.mtx.tx);
							keyframe->QueryFloatAttribute("ty", &key.mtx.ty);
							keyframe->QueryFloatAttribute("alpha", &key.alpha);

							inst.keyframes.push_back(key);

							keyframe = keyframe->NextSiblingElement();
						}
					}

					instElem = instElem->NextSiblingElement();
					lay.mInstances.push_back(inst);
				}

				mod.layers.push_back(lay);
				layerElem = layerElem->NextSiblingElement();
			}

			mModels[mod.name] = mod;

			if(mod.name == "root")
			{
				this->root = &mModels[mod.name];
			}

			elem = elem->NextSiblingElement();
		}

		// Création des liens Instance / Bitmap, Model
		for(std::map<std::string, DOMModel>::iterator i = mModels.begin(); i != mModels.end(); i++)
		{
			for(std::vector<DOMLayer>::iterator k = i->second.layers.begin(); k != i->second.layers.end(); k++)
			{
				for(std::vector<DOMInstance>::iterator j = k->mInstances.begin(); j != k->mInstances.end(); j++)
				{
					if(j->instance_type == DOM_INSTANCE_TYPE_BITMAP)
					{
						j->of = &mBitmaps[j->of_name];
					}
					else if(j->instance_type == DOM_INSTANCE_TYPE_MODEL)
					{
						j->of = &mModels[j->of_name];
					}
				}
			}
		}
	}

	DOMNode* ExtendedSpriteRessource::FindNodeByName(std::string name)
	{
		// On cherche dans le models
		for(std::map<std::string, DOMBitmap>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); it++)
		{
			if(it->first == name)
			{
				return (DOMNode*)&(it->second);
			}
		}

		for(std::map<std::string, DOMModel>::iterator it = mModels.begin(); it != mModels.end(); it++)
		{
			if(it->first == name)
			{
				return (DOMNode*)&(it->second);
			}
		}

		return NULL;
	}
}