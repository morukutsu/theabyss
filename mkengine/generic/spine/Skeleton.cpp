#include <iostream>
#include "Skeleton.h"
#include <spine/SkeletonData.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Atlas.h"

#include "mkengine.h"

using sf::Quads;
using sf::RenderTarget;
using sf::RenderStates;

namespace spine {

Skeleton::Skeleton () 
{
	currentAnim = NULL;
}

Skeleton::~Skeleton()
{
	delete atlas_file;
	delete baseSkel;
}

void Skeleton::Draw ()
{
	vertexArray.clear();
	for (int i = 0, n = baseSkel->slots.size(); i < n; i++)
		if (baseSkel->slots[i]->attachment) 
		{
			baseSkel->slots[i]->attachment->draw(baseSkel->slots[i]);

			RegionAttachment* region = (RegionAttachment*)baseSkel->slots[i]->attachment;
			region->finish(vertexArray, &texture);
		}
	
	baseSkel->updateWorldTransform();
	lowDisplayVtxArray(vertexArray, texture, posX, posY, posZ, scaleX, scaleY, angle, mirrorX, mirrorY);
}

void Skeleton::Interpolate(float dt)
{
	currentAnim->apply(baseSkel, animationTime + (1.0f/30.0f)*dt, true);
}

void Skeleton::PlayAnim(std::string anim)
{
	currentAnim = animations[anim];
	animationTime = 0;
}

void Skeleton::Play()
{
	animationTime += 1.0f/30.0f;
}


void Skeleton::LoadModelDescriptorFile(std::string filename)
{
	mk::AsciiFile * f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);

	TiXmlDocument doc;

	doc.Parse(f->getString() );

	TiXmlElement * root = doc.FirstChildElement("SpineModel");
	TiXmlElement * elem;

	// Clearing our data structures

	// Reading from XML file
	std::string atlas, skeleton;

	elem = root->FirstChildElement("Atlas");
	atlas = elem->GetText();

	elem = root->FirstChildElement("Skeleton");
	skeleton = elem->GetText();

	// Chargement de l'Atlas
	mk::AsciiFile* atlasFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(atlas);
	std::string tmp = atlasFile->getString();
	tmp = tmp.substr(0, atlasFile->getSize());

	atlas_file = new spine::Atlas(tmp);
	spine::SkeletonJson skeletonJson(atlas_file);

	mk::RessourceManager::getInstance()->DeleteRessource(atlasFile);

	// Chargement du squelette
	mk::AsciiFile* skeletonFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(skeleton);
	tmp = skeletonFile->getString();
	tmp = tmp.substr(0, skeletonFile->getSize());

	skeletonData = skeletonJson.readSkeletonData(tmp);
	
	mk::RessourceManager::getInstance()->DeleteRessource(skeletonFile);

	// Loading animations
	elem = root->FirstChildElement("Animations")->FirstChildElement();
	while(elem) 
	{
		std::string name = elem->Attribute("name");
		std::string filename = elem->Attribute("filename");

		mk::AsciiFile* animationFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);
		tmp = animationFile->getString();
		tmp = tmp.substr(0, animationFile->getSize());

		spine::Animation* animation = skeletonJson.readAnimation(tmp, skeletonData);

		animations[name] = animation;

		mk::RessourceManager::getInstance()->DeleteRessource(animationFile);

		elem = elem->NextSiblingElement();
	}

	// Chargement du squelette
	baseSkel = new BaseSkeleton(skeletonData);
	baseSkel->flipX = false;
	baseSkel->flipY = false;
	baseSkel->setToBindPose();
	baseSkel->getRootBone()->x = 0;
	baseSkel->getRootBone()->y = 0;
	baseSkel->updateWorldTransform();

	// Initialisation
	vertexArray.setPrimitiveType(Quads);
	vertexArray.resize(skeletonData->bones.size() * 4);
}

}
