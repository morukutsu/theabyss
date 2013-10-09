// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "CutsceneEventFx.h"
#include <iostream>
#include "level_manager\LevelManager.h"

CutsceneEventFx::CutsceneEventFx(LevelManager* lvlMan, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters) 
	: CutsceneEvent(lvlMan, eventType, time, duration) 
{
	for(std::map<std::string, std::string>::iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		if((*it).first == "fx_type")
		{
			fx_type = (*it).second;
		}
		else if((*it).first == "intensity")
		{
			sscanf((*it).second.c_str(), "%f", &intensity_start);
		}
		else if((*it).first == "toggle")
		{
			if((*it).second == "on")
				toggle = true;
			else
				toggle = false;
		}
		else if((*it).first == "color_r")
		{
			sscanf((*it).second.c_str(), "%d", &r);
		}
		else if((*it).first == "color_g")
		{
			sscanf((*it).second.c_str(), "%d", &g);
		}
		else if((*it).first == "color_b")
		{
			sscanf((*it).second.c_str(), "%d", &b);
		}
	}
}

void CutsceneEventFx::Start() 
{
	if(fx_type == "blur") 
	{
		if(toggle)
		{
			levelManager->scene->blur_intensity = intensity_start;
			levelManager->scene->isEffectBlur = true;
		}
		else
		{
			levelManager->scene->isEffectBlur = false;
		}
	}
	else if(fx_type == "black_bands")
	{
		levelManager->scene->InitBlackBands();
		levelManager->scene->ActivateBlackBands(toggle);
	}
	else if(fx_type == "plane")
	{
		// Plan pour fadein fade out
		levelManager->scene->InitPlane(toggle, r, g, b, intensity_start);
	}
	else if(fx_type.find("shader") != std::string::npos)
	{
		if(toggle)
		{
			// Un shader est detecté
			std::string prefix = "shader_";
			std::string shaderName = fx_type.substr(prefix.length() );

			if(shaderName == "filterA")
				levelManager->scene->LoadPostFXShader("shaders/postfx_filterA_lookup.fx");
			else if(shaderName == "filterB")
				levelManager->scene->LoadPostFXShader("shaders/postfx_filterB_lookup.fx");
			else if(shaderName == "filterC")
				levelManager->scene->LoadPostFXShader("shaders/postfx_filterC_lookup.fx");
			else if(shaderName == "filterD")
				levelManager->scene->LoadPostFXShader("shaders/postfx_filterD_lookup.fx");
			else if(shaderName == "filterE")
				levelManager->scene->LoadPostFXShader("shaders/postfx_filterE_lookup.fx");
			else if(shaderName == "grayscale")
				levelManager->scene->LoadPostFXShader("shaders/postfx_grayscale.fx");
			else if(shaderName == "saturation")
				levelManager->scene->LoadPostFXShader("shaders/postfx_saturation.fx");
			else if(shaderName == "scanline")
				levelManager->scene->LoadPostFXShader("shaders/postfx_scanline.fx");
			else if(shaderName == "sepia")
				levelManager->scene->LoadPostFXShader("shaders/postfx_sepia.fx");

			levelManager->scene->SetEffectIntensity(intensity_start);
		}
		else
		{
			levelManager->scene->isPostFXShader = false;
		}
	}
}

void CutsceneEventFx::Run(float time) 
{
}

void CutsceneEventFx::End() 
{

}