// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Scene.h"
#include "mkengine.h"
#include "Sprite.h"

namespace mk
{
	bool DrawableSortPredicateBackToFront(const Drawable *a, const Drawable *b)
	{
		bool r;
		// Checking equality with 1/8,000,000 and 1/16,000,000 precision
		// 32 bit compilation mandatory !
		/*if(AlmostEqual2sComplement(a->posZ, b->posZ, 1))
		{
			r = a->priority > b->priority;
		}
		else
		{*/
		r = a->posZ + a->priority*PRIORITY_FACTOR > b->posZ + b->priority*PRIORITY_FACTOR;
		/*}*/

		return r;
	}

	bool DrawableSortPredicateFrontToBack(const Drawable *a, const Drawable *b)
	{
		bool r;
		// Checking equality with 1/8,000,000 and 1/16,000,000 precision
		// 32 bit compilation mandatory !
		/*if(AlmostEqual2sComplement(a->posZ, b->posZ, 1))
		{
			r = a->priority > b->priority;
		}
		else
		{*/
			r = a->posZ + a->priority*0.01f < b->posZ + b->priority*0.01f;
		/*}*/

		return r;
	}

	Scene::Scene()
	{
		elements.clear();
		//lights.clear();

		// TODO : conserver les FBO en mémoire, ou ne pas détruire la scène dans le lvl manager ...
		// Pipeline
		mWorkFBO = mk::FBO::getInstance();

		isLighting = false;
		isShowLightMask = false;
		isShowAABB = false;
		isPostFXShader = false;

		windowAABB.x1;
		windowAABB.x2;
		windowAABB.y1;
		windowAABB.y2;

		debugFont = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");

		InitBlur();
		isEffectBlur = false;

		isBlackBandsActivated = false;
		isBlackBandsDisplayed = false;

		// Chargement du shader fix lights
		light_alpha_fix = (mk::Shader*)mk::RessourceManager::getInstance()->LoadRessource("shaders/light_alpha_fix.fx");
		light_alpha_fix->shader.setParameter("texture", sf::Shader::CurrentTexture);

		effectIntensity = 1.0f;

		// Compilation de certains shaders
		mk::RessourceManager::getInstance()->LoadRessource("shaders/postfx_blur_h.fx");
		mk::RessourceManager::getInstance()->LoadRessource("shaders/postfx_blur_v.fx");
	}

	Scene::~Scene()
	{
		//mWorkFBO->Destroy();
		//delete mWorkFBO;

		for(std::list<Drawable*>::iterator it = elements.begin(); it != elements.end(); )
		{
			elements.erase(it++);
		}

		elements.clear();
	}

	void Scene::Add(Drawable* elem)
	{
		elem->Show();
		elem->ignoreLightPipeline = false;
		elements.push_back(elem);
	}
	
	void Scene::Remove(Drawable* elem)
	{
		elements.remove(elem);
	}
	
	void Scene::AddLight(Drawable* elem)
	{
		elem->Show();
		elem->SetType(DRAWABLE_TYPE_LIGHT);
		elem->SetBlending(MK_BLEND_ADD);
		elements.push_back(elem);
	}

	void Scene::RemoveLight(Drawable* elem)
	{
		elements.remove(elem);
	}
	
	void Scene::SplitElementsList(std::list<Drawable*>& opaques, std::list<Drawable*>& transparents)
	{
		for(std::list<Drawable*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if((*it)->isVisible && !(*it)->isCulled) {
				if((*it)->isOpaque)
				{
					opaques.push_back((*it));
				}
				else
				{
					transparents.push_back((*it));
				}
			}
		}

		// Tri des elements de la scene
		transparents.sort(DrawableSortPredicateBackToFront);
		opaques.sort(DrawableSortPredicateFrontToBack);
	}

	void Scene::Draw(float dt)
	{
		std::list<Drawable*> opaques;
		std::list<Drawable*> transparents;

		SplitElementsList(opaques, transparents);

		// Frustum culling
		mDisplayedElements = 0;
		FrustumCulling();

		// Interpolation de la scene
		Interpolation(dt);

		// Light Pass : Ecrit les lumières dans un FBO
		if(isLighting)
		{
			lowSetBlendMode(MK_BLEND_ADD);
			mWorkFBO->StartDrawing(0);

			glClearDepth(1.0f);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glStencilMask(0xFF);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			StencilWritePass(elements);

			LightPass(opaques);
			LightPass(transparents);

			mWorkFBO->EndDrawing();
		}

		// Diffuse Pass
		lowSetBlendMode(MK_BLEND_ALPHA);
		mWorkFBO->StartDrawing(1);
		/*glClearDepth(1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

		DiffusePass(opaques);
		DiffusePass(transparents);

		if(isShowAABB)
			DebugDrawWindowAABB();
		
		// Light Pass End
		if(isLighting)
		{
			ApplyLights();
		}

		mWorkFBO->EndDrawing();
	}

	void Scene::DrawBackground()
	{
		if(backgroundSprite != NULL)
		{
			// Diffuse Pass
			mWorkFBO->StartDrawing(1);

			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);

			lowSetBlendMode(MK_BLEND_ALPHA);
			glDepthMask(GL_FALSE);
			backgroundSprite->MoveTo(0, 0);
			backgroundSprite->Draw();
			glDepthMask(GL_TRUE);

			mWorkFBO->EndDrawing();
		}
	}

	void Scene::FinalDraw()
	{
		if(isEffectBlur)
			RenderBlur(blur_intensity);

		glPushMatrix();

		// Cas avec seulement un postfx d'activé
		mk::Core::ConfigureViewportFBO();
		lowSetBlendMode(MK_BLEND_ALPHA);

		mWorkFBO->Bind(1);
		
		if(isPostFXShader)
		{
			postfx->shader.setParameter("intensity", effectIntensity);
			postfx->Bind();
		}

		lowDisplayFBO(mWorkFBO, 1);

		if(isPostFXShader)
			postfx->Unbind();

		UpdateBlackBands();
		RenderBlackBands(0.0f);

		mk::Core::ConfigureViewport();

		glPopMatrix();
	}

	void Scene::LoadPostFXShader(std::string shader)
	{
		// Chargement du shader
		postfx = (mk::Shader*)mk::RessourceManager::getInstance()->LoadRessource(shader);
		postfx->shader.setParameter("texture", sf::Shader::CurrentTexture);

		// On cherche si le postfx contient une lookup
		if(shader.find("lookup") != std::string::npos) {
			// On charge la lookuptable
			std::string lookupfile = shader;
			lookupfile = lookupfile.substr(0, shader.length() - 2) + "png";
			mk::Image* lookupimg = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(lookupfile);
			postfx->shader.setParameter("lookup", lookupimg->img);
		}

		isPostFXShader = true;
	}

	void Scene::FrustumCulling()
	{
		for(std::list<Drawable*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if((*it)->isCullingIgnored)
			{
				(*it)->isCulled = false;
			}
			else 
			{
				BoundingBox window = TransformWindowAABB((*it)->posZ);
				if(AABBCollide((*it)->posX, (*it)->posY, &(*it)->GetTransformedBoundingBox(), 0, 0, &window) ) 
				{
					(*it)->isCulled = false;
				} 
				else 
				{
					(*it)->isCulled = true;
				}
			}
		}
	}

	void Scene::Interpolation(float dt)
	{
		for(std::list<Drawable*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if((*it)->isVisible && !(*it)->isCulled) {
				(*it)->Interpolate(dt);
				mDisplayedElements++;
			}
		}
	}

	void Scene::StencilWritePass(std::list<Drawable*>& list)
	{
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glDepthMask(GL_FALSE);
		
		// A ajuster afin d'éviter que ça pixellise
		glAlphaFunc(GL_GREATER, 0.75);

		glStencilFunc( GL_ALWAYS, 1, 0xFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
		glStencilMask( 0xFF );

		for(std::list<Drawable*>::iterator it = list.begin(); it != list.end(); it++)
		{
			if((*it)->mType == DRAWABLE_TYPE_MAP && (*it)->posZ <= 0.0f)
			{
				(*it)->Draw();
			}
		}

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilMask(0x00);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_ALPHA_TEST);
	}

	void Scene::LightPass(std::list<Drawable*>& list)
	{
		// Render geometry in black and lights
		for(std::list<Drawable*>::iterator it = list.begin(); it != list.end(); it++)
		{
			if((*it)->mType == DRAWABLE_TYPE_SIMPLE || (*it)->mType == DRAWABLE_TYPE_MAP)
			{
				if((*it)->isVisible && (*it)->ignoreLightPipeline == false && !(*it)->isCulled)
				{
					lowSetBlendMode(MK_BLEND_ALPHA);

					if((*it)->posZ >= 0.0f)
						light_alpha_fix->Bind();

					unsigned char r, g, b;
					r = (*it)->r, g = (*it)->g, b = (*it)->b;
					(*it)->r = (*it)->g = (*it)->b = 0;
					(*it)->Draw();
					(*it)->r = r, (*it)->g = g, (*it)->b = b;

					if((*it)->posZ >= 0.0f)
						light_alpha_fix->Unbind();
				}
			}
			else if((*it)->mType == DRAWABLE_TYPE_LIGHT)
			{
				if((*it)->isVisible && !(*it)->isCulled)
				{
					lowSetBlendMode((*it)->blending);

					if((*it)->isStencilClipped) 
					{
						glEnable(GL_STENCIL_TEST);
						glStencilFunc(GL_EQUAL, 0, 0xFF);
						glStencilMask(0x00);
					}

					(*it)->Draw();

					if((*it)->isStencilClipped) 
					{
						glDisable(GL_STENCIL_TEST);
					}

				}
			}
		}
	}

	void Scene::DiffusePass(std::list<Drawable*>& list)
	{
		for(std::list<Drawable*>::iterator it = list.begin(); it != list.end(); it++)
		{
			if((*it)->mType == DRAWABLE_TYPE_SIMPLE || (*it)->mType == DRAWABLE_TYPE_MAP)
			{
				if((*it)->isVisible && !(*it)->isCulled)
				{
					lowSetBlendMode((*it)->blending);
					(*it)->Draw();
					if(isShowAABB)
						(*it)->DrawBoundingBox();
				}
			}
			else if((*it)->mType == DRAWABLE_TYPE_LIGHT)
			{
				if((*it)->isVisible && !(*it)->isCulled)
				{
					if(isShowAABB)
						(*it)->DrawBoundingBox();
				}
			}
		}
	}

	void Scene::ApplyLights()
	{
		glPushMatrix();

		mk::Core::ConfigureViewportFBO();

		// Permet de debugguer en affichant le masque des lumières
		if(!isShowLightMask)
			lowSetBlendMode(MK_BLEND_MULTIPLY);
		else
			lowSetBlendMode(MK_BLEND_ALPHA);

		lowDisplayFBO(mWorkFBO, 0);

		lowSetBlendMode(MK_BLEND_ALPHA);
			
		mk::Core::ConfigureViewport();

		glPopMatrix();
	}

	void Scene::Clear() 
	{
		for(std::list<Drawable*>::iterator it = elements.begin(); it != elements.end(); )
		{
			elements.erase(it++);
		}
	}
	
	void Scene::SetCullOrigin(float _cx, float _cy)
	{
		cullOriginX = _cx;
		cullOriginY = _cy;
		
		windowAABB.x1 = - mk::Core::getBaseWidth()/2.0f/32.0f;
		windowAABB.x2 = + mk::Core::getBaseWidth()/2.0f/32.0f;
		windowAABB.y1 = - mk::Core::getBaseHeight()/2.0f/32.0f;
		windowAABB.y2 = + mk::Core::getBaseHeight()/2.0f/32.0f;
	}

	BoundingBox Scene::TransformWindowAABB(float z)
	{
		BoundingBox window = windowAABB;

		// Correction de la taille de l'AABB en fonciton de la profondeur de l'élément graphique
		if(z > 0.0f) {
			float factor = 1.0f + (z / 20.0f);
			window.x1 *= factor;
			window.x2 *= factor;
			window.y1 *= factor;
			window.y2 *= factor;
		}

		window.x1 += cullOriginX;
		window.y1 += cullOriginY;
		window.x2 += cullOriginX;
		window.y2 += cullOriginY;

		return window;
	}

	void Scene::SetBackground(mk::Sprite* bg)
	{
		backgroundSprite = bg;
	}

	void Scene::InitBlur()
	{
		// Chargement du shader horizontal
		shader_blurH = (mk::Shader*)mk::RessourceManager::getInstance()->LoadRessource("shaders/postfx_blur_h.fx");
		shader_blurH->shader.setParameter("texture", sf::Shader::CurrentTexture);

		// Chargement du shader vertical
		shader_blurV = (mk::Shader*)mk::RessourceManager::getInstance()->LoadRessource("shaders/postfx_blur_v.fx");
		shader_blurV->shader.setParameter("texture", sf::Shader::CurrentTexture);
	}

	void Scene::RenderBlur(float intensity)
	{
		// Utilise la mémoire réservée pour le FBO light
		// A appliquer après le FBO light

		glPushMatrix();

		// Cas avec seulement un postfx d'activé
		mk::Core::ConfigureViewportFBO();
		lowSetBlendMode(MK_BLEND_ALPHA);

		// Biding current scene in FBO 1
		mWorkFBO->Bind(1);
		
		shader_blurH->shader.setParameter("intensity", effectIntensity);
		shader_blurH->shader.setParameter("radius", intensity);
		shader_blurH->Bind();

		// Displaying in FBO 0
		mWorkFBO->StartDrawing(0);
		lowDisplayFBO(mWorkFBO, 1);
		mWorkFBO->EndDrawing();

		shader_blurH->Unbind();

		// Biding H blurred scene in FBO 0
		mWorkFBO->Bind(0);

		shader_blurV->shader.setParameter("intensity", effectIntensity);
		shader_blurV->shader.setParameter("radius", intensity);
		shader_blurV->Bind();

		// Displaying in FBO 1
		mWorkFBO->StartDrawing(1);
		lowDisplayFBO(mWorkFBO, 0);
		mWorkFBO->EndDrawing();

		shader_blurV->Unbind();

		mk::Core::ConfigureViewport();

		glPopMatrix();
	}

	void Scene::InitBlackBands()
	{
		black_bands_time = 0.0f;
	}

	void Scene::RenderBlackBands(float interp)
	{
		if(isBlackBandsActivated)
		{
			lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), black_bands_h, 0x000000FF);
			lowDisplayFillRect(0, mk::Core::getBaseHeight(), mk::Core::getBaseWidth(), mk::Core::getBaseHeight() - black_bands_h, 0x000000FF);
		}
	}

	void Scene::ActivateBlackBands(bool toggle)
	{
		isBlackBandsActivated = true;
		blackbandsSens = toggle;

		if(toggle)
		{
			black_bands_h = 0.0f;
		}
		else
		{
			black_bands_h = BLACK_BARS_HEIGHT;
		}
	}

	void Scene::UpdateBlackBands()
	{
		black_bands_time += 1.0f/30.0f;
		if(black_bands_time > 2.0f)
		{
			black_bands_time = 2.0f;

			if(blackbandsSens == false)
			{
				isBlackBandsActivated = false;
			}
		}

		old_black_bands_h = black_bands_h;

		if(blackbandsSens)
		{
			black_bands_h = Lerp(black_bands_time, 0.0f, 2.0f, 0.0f, BLACK_BARS_HEIGHT);
		}
		else
		{
			black_bands_h = Lerp(black_bands_time, 0.0f, 2.0f, BLACK_BARS_HEIGHT, 0.0f);
		}
	}

	void Scene::ShowDebugInfo(float x, float y)
	{
		char text[64];
		sprintf(text, "Displayed elements : %d / %d", mDisplayedElements, elements.size() );
		lowDisplayText(debugFont, x, y, text, 0xAAAAAAFF, 0.8f);
	}

	void Scene::DebugDrawWindowAABB()
	{
		BoundingBox box = windowAABB;

		glPushMatrix();
		
		glTranslatef(0, 0, 0);

		glDisable (GL_TEXTURE_2D);
		glEnable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);

		glColor3f (0.0f, 1.0f, 0.0f);

		glBegin (GL_LINE_STRIP);

		glVertex3f(box.x1, box.y1, 0);
		glVertex3f(box.x2, box.y1, 0);
		glVertex3f(box.x2, box.y2, 0);
		glVertex3f(box.x1, box.y2, 0);
		glVertex3f(box.x1, box.y1, 0);

		glEnd();

		glDisable (GL_BLEND);
		glDisable (GL_DEPTH_TEST);

		glPopMatrix();
	}
};