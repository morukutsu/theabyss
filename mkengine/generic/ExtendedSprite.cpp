// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "ExtendedSpriteRessource.h"
#include "ExtendedSprite.h"
#include "lowDisplay.h"
#include "lowSystem.h"

#include <iostream>

namespace mk
{
	ExtendedSprite::ExtendedSprite() : Drawable()
	{
		centerX = centerY = 0;
        posX = posY = 0;
        scaleX = scaleY = 1.0f;
        angle = 0.0f;
        mirrorX = mirrorY = false;
        alpha = 1.0f;
		priority = 0;
		res = NULL;
	}

	ExtendedSprite::~ExtendedSprite()
	{

	}

	void ExtendedSprite::Assign(ExtendedSpriteRessource* pRes)
	{
		res = pRes;

		// On charge tout les modèles a partir du Root
		rootNode = CreateNodeFromName("root");
	}

	ExtSprNode* ExtendedSprite::CreateNodeFromName(std::string name)
	{
		// Finding what node we have to create
		DOMNode* ressourceNode = res->FindNodeByName(name);
		if(ressourceNode == NULL)
			return NULL;

		std::cout << "Found object ressource : " << ressourceNode->name << std::endl;

		// Création de l'arborescence
		ExtSprNode* node;

		// Création d'un nouveau node
		if(ressourceNode->node_type == DOM_INSTANCE_TYPE_BITMAP)
		{
			std::cout << "-- Bitmap" << std::endl;
			node = new ExtSprBitmap(this, (DOMBitmap*)ressourceNode);
		}
		else
		{
			std::cout << "-- Model" << std::endl;
			node = new ExtSprModel(this, (DOMModel*)ressourceNode);	
		}

		return node;
	}

	void ExtendedSprite::Draw()
	{
		lowPushMatrix();

		lowSetCurrentColor(1.0f, 1.0f, 1.0f, alpha);
		lowTranslate(posX, posY);
		rootNode->Draw();

		lowPopMatrix();
	}

	void ExtendedSprite::Interpolate(float dt)
	{

	}

	void ExtendedSprite::Rotate(float Angle)
    {
        angle = Angle;
    }

    void ExtendedSprite::MoveTo(float x, float y)
    {
        posX = x;
        posY = y;
    }

	// ---- ExtSprModel
	ExtSprModel::ExtSprModel(ExtendedSprite* main, DOMModel* model) 
	{
		// Initalisation variables
		currentKeyFrame = 0;
		nextKeyFrame = 1;
		centerX = centerY = 0;
		frameCounter = 0;
		curFrameTime = 0;
		lastTime = 0.0f;
		inst = NULL;

		animStarter = -1;

		modelRes = model;
		
		isRoot = false;

		// Listing des instances dans ce modèle à charger
		for(std::vector<mk::DOMLayer>::iterator i = model->layers.begin(); i != model->layers.end(); i++)
		{
			for(std::vector<DOMInstance>::iterator iInst = i->mInstances.begin(); iInst != i->mInstances.end(); iInst++)
			{
				ExtSprNode* node = main->CreateNodeFromName(iInst->of_name);
				
				node->mtx.a = iInst->a;
				node->mtx.b = iInst->b;
				node->mtx.c = iInst->c;
				node->mtx.d = iInst->d;
				node->mtx.tx = iInst->tx;
				node->mtx.ty = iInst->ty;
				node->centerX = iInst->centerX;
				node->centerY = iInst->centerY;
				node->inst = &(*iInst);
				
				childs.push_front(node);
			}
		}
	}

	void ExtSprModel::Draw() 
	{
		Matrix currentMatrix;
		float alphaValue = 1.0f;

		// Lecture de l'animation
		if(inst != NULL)
		{
			if(inst->hasMotionObject)
			{
				Play(lowGetFrameTime() );

				int length;
				if(currentKeyFrame == inst->keyframes.size() - 1)
					length = inst->duration - inst->keyframes[currentKeyFrame].time;
				else
					length = inst->keyframes[nextKeyFrame].time - inst->keyframes[currentKeyFrame].time;

				//std::cout << inst->fps << "FPS, len : " << length << " frametime : " << curFrameTime << "/" << (1.0f/inst->fps)*length << std::endl;
				
				
				// TODO : Loop mode or not loop mode ?
				if(currentKeyFrame == inst->keyframes.size() - 1)
				{
					currentMatrix = inst->keyframes[currentKeyFrame].mtx;
					alphaValue = inst->keyframes[currentKeyFrame].alpha;
				}
				else
				{
					// Interpolation des matrices
					currentMatrix = Matrix::InterpolateMtx(inst->keyframes[currentKeyFrame].mtx, 
						inst->keyframes[nextKeyFrame].mtx, curFrameTime, (1.0f/inst->fps)*length);
					
					//currentMatrix = inst->keyframes[currentKeyFrame].mtx;

					// Interpolation de l'alpha
					alphaValue = Lerp(curFrameTime, 0.0f, (1.0f/inst->fps)*length, 
						inst->keyframes[currentKeyFrame].alpha, inst->keyframes[nextKeyFrame].alpha);
				}

				currentMatrix.Concat(mtx);
			}
			else
			{
				currentMatrix = mtx;
				alphaValue = 1.0f; // TODO : charger l'alpha quand il n'y a pas de motion object
			}
		}

		// On part court les enfants et on les affiche
		bool toDisplay = true;

		// On affiche pas l'instance si on animStarter est différent de 0
		if(animStarter > 0)
			toDisplay = false;

		if(toDisplay)
		{
			lowPushMatrix();

			// Transformation
			lowTranslate(currentMatrix.tx-centerX, currentMatrix.ty-centerY);	
			lowScale(currentMatrix.GetScaleX(), currentMatrix.GetScaleY() );
			
			lowRotate((currentMatrix.GetAngle()*180.0f) / PI );
			

			// Couleur
			lowSetCurrentColor(1.0f, 1.0f, 1.0f, alphaValue/* * lowGetCurrentAlpha() */);

			// Affichage des enfants
			for(std::list<ExtSprNode*>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				(*it)->Draw();
			}

			lowPopMatrix();
		}
	}

	void ExtSprModel::Play(float dt)
	{
		// Gestion du starter
		if(animStarter == -1)
		{
			animStarter = inst->animStart;
		}

		// Pour gérer le cas où il n'y a qu'une seule frame d'animation
		if(nextKeyFrame >= inst->keyframes.size())
			nextKeyFrame = 0;

		lastTime += dt;
		
		// On change de frame à chaque fois qu'un pas du FPS est atteint
		if(lastTime >= 1.0f/inst->fps)
		{
			lastTime = 0.0f;

			if(animStarter == 0)
			{
				frameCounter++;	
			}
			else
			{
				animStarter--;
			}
		}

		// Si l'animation a démarrée
		if(animStarter == 0)
		{
			curFrameTime += dt;

			if(frameCounter >= inst->duration)
				frameCounter = 0;

			if(frameCounter == inst->keyframes[nextKeyFrame].time)
			{
				currentKeyFrame++;
				nextKeyFrame++;

				curFrameTime = 0.0f;

				if(currentKeyFrame >= inst->keyframes.size())
				{
					currentKeyFrame = 0;
					animStarter = -1;
				}

				if(nextKeyFrame >= inst->keyframes.size())
					nextKeyFrame = 0;		
			}
		}
	}

	// ---- ExtSprBitmap
	ExtSprBitmap::ExtSprBitmap(ExtendedSprite* main, DOMBitmap* bitmap) 
	{
		bitmapRes = bitmap;
		isRoot = false;

	}

	void ExtSprBitmap::Draw() 
	{
		// On affiche le bitmap !
		//lowPushMatrix();

		// Transformation
		//lowTranslate(mtx.tx-centerX, mtx.ty-centerY);
		//lowRotate((mtx.GetAngle()*180.0f) / PI );
		//lowScale(mtx.GetScaleX(), mtx.GetScaleY() );

		lowDisplayQuadTex(bitmapRes->bitmap);

		//lowPopMatrix();
	}
}