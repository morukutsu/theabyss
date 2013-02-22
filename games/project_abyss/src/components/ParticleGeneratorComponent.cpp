// project_abyss
// LittleSpace Studio 2012

#include "ParticleGeneratorComponent.h"
#include "../entities/Entity.h"
#include "../SimpleMaths.h"

RangeFunc* createFunc(std::string name)
{
	if(name == "sin")
		return new RangeFuncSin();
	else if(name == "random")
		return new RangeFuncRandom();
}

ParticleGeneratorComponent::ParticleGeneratorComponent(std::string filename)
{
	// Intialisation
	maxParticles = -1;
	initialLife = 1.0f;
	initialSize = 1.0f;
	gravity = 0.0f;
	particleBootTime = -1.0f;
	isActive = true;
	initialAngle = 0.0f;
	genTime = 0.0f;
	offsetX = offsetY = offsetZ = 0.0f;
	movingParent_vx = movingParent_vy = movingParent_vz = 0.0f;
	priority = 0;
	rotCenterX = 0;
	rotCenterY = 0;
	rotAngle = 0;

	// Initialisations range funcs
	for(int i = 0; i < MAX_PARAMETERS; i++)
		rangeFuncs[i] = NULL;

	// Parsing du fichier XML contenant les particules
	mk::AsciiFile * f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);
	TiXmlDocument doc;
	doc.Parse(f->getString() );

	TiXmlElement * root = doc.FirstChildElement("Generator");

	TiXmlElement * elem = root->FirstChildElement("Parameter");
	while(elem)
	{
		std::string parameterName = elem->Attribute("name");

		if(parameterName == "Sprite")
		{
			spriteFilename = elem->Attribute("value");
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

		elem = elem->NextSiblingElement();
	}

	// Preloading particle sprite
	img = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("particles/" + spriteFilename);
}

ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
	for(int i = 0; i < MAX_PARAMETERS; i++)
	{
		if(rangeFuncs[i])
			delete rangeFuncs[i];
	}

	particles.swap(particles);
	particles.clear();
}

void ParticleGeneratorComponent::Init()
{
	// Creating particles array
	if(maxParticles != -1)
	{
		particles.resize(maxParticles);
		for(int k = 0; k < maxParticles; k++) 
		{
			particles[k].active = false;
			particles[k].spr.Assign(img);
			particles[k].spr.Set3DMode(true);
			particles[k].spr.isCullingIgnored = true;

			// Ajout des sprites au Scene Node
			parent->GetScene()->Add(&particles[k].spr);

			particles[k].spr.Hide();
			particles[k].spr.SetPriority(priority);
			particles[k].spr.ignoreLightPipeline = ignoreLightPipeline;

			//parent->GetScene()->AddLight(&particles[k].spr);
		}
	}
}

void ParticleGeneratorComponent::ChangePriority(int prio)
{
	for(int k = 0; k < maxParticles; k++) 
	{
		particles[k].spr.SetPriority(prio);
	}
	priority = prio;
}

void ParticleGeneratorComponent::SetIgnoreLightPipeline(bool t)
{
	for(int k = 0; k < maxParticles; k++) 
	{
		particles[k].spr.ignoreLightPipeline = t;
	}
}

void ParticleGeneratorComponent::Receive(int message, void* data)
{

}

void ParticleGeneratorComponent::Update()
{
	for(int k = 0; k < maxParticles; k++) 
	{
		if(particles[k].active)
		{
			// Update des paramètres des particules
			particles[k].x += particles[k].vx;
            particles[k].y += particles[k].vy;
			particles[k].z += particles[k].vz;
			particles[k].alpha += particles[k].alphaFade;
			particles[k].angle += particles[k].angleVariation;
			particles[k].size += particles[k].sizeVariation;

			particles[k].life -= particles[k].fade;
			particles[k].vy += gravity;
			particles[k].spr.Show();
			if (particles[k].life <= 0.0f)
			{
				particles[k].active = false;
				particles[k].spr.Hide();
			}

			if(particles[k].size < 0.0f)
				particles[k].size = 0.0f;

			// Déplacement du sprite dans le scene node
			particles[k].spr.SetSize((img->getImageWidth() / 32.0f) * particles[k].size, 
					(img->getImageHeight() / 32.0f) * particles[k].size);

			if(!referencial)
			{
				NVector targetPoint = NVector(particles[k].x/32.0f, particles[k].y/32.0f);
				NVector center = NVector(rotCenterX, rotCenterY);
				targetPoint.Rotate(center, DegreesToRadians(rotAngle) );

				float mx = mirrorV ? -1.0f: 1.0f;
				float my = mirrorH ? -1.0f: 1.0f;

				particles[k].spr.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
			}
			else
			{
				particles[k].spr.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
			}

			particles[k].spr.SetDepth(particles[k].z + offsetZ);
			particles[k].spr.Alpha(particles[k].alpha);
			particles[k].spr.Rotate(particles[k].angle);
			particles[k].spr.Tint((unsigned char)(particles[k].r*255), (unsigned char)(particles[k].g*255), (unsigned char)(particles[k].b*255));
			particles[k].spr.SavePositions();

			// On empêche l'interpolation durant cette frame
			if(oldMirrorV != mirrorV || oldMirrorH != mirrorH)
				particles[k].spr.SavePositions();
		}
		else
		{
			// Ajout d'une nouvelle particule
			if (particleBootTime <= 0.0f && isActive)
			{
				particleBootTime = 1.0f/frequency;

				particles[k].life = initialLife;
				particles[k].fade = fade + SimpleMaths::Rand(fade_rmin, fade_rmax);
                particles[k].active = true;

				if(referencial == false)
				{
					particles[k].x = 0; 
					particles[k].y = 0;
					particles[k].z = 0;
				}
				else
				{
					particles[k].x = parent->mPos.x; 
					particles[k].y = parent->mPos.y;
					particles[k].z = parent->mDepth;
				}
               
				particles[k].vx = vx + rangeFuncs[PARAM_VEL_X]->func(genTime, vx_rmin, vx_rmax) + movingParent_vx;
				particles[k].vy = vy + rangeFuncs[PARAM_VEL_Y]->func(genTime, vy_rmin, vy_rmax) + movingParent_vy;
				particles[k].vz = vz + rangeFuncs[PARAM_VEL_Z]->func(genTime, vz_rmin, vz_rmax) + movingParent_vz;
				particles[k].alpha = initialAlpha;
				particles[k].alphaFade = alphaFade;
				particles[k].angle = alphaFade;
				particles[k].angleVariation = angleVariation;

				particles[k].size = initialSize;
				particles[k].sizeVariation = sizeVariation;

				particles[k].r = initialColorR;
				particles[k].g = initialColorG;
				particles[k].b = initialColorB;

				
				particles[k].spr.SetSize((img->getImageWidth() / 32.0f) * particles[k].size, 
					(img->getImageHeight() / 32.0f) * particles[k].size);

				if(!referencial)
				{
					NVector targetPoint = NVector(particles[k].x/32.0f, particles[k].y/32.0f);
					NVector center = NVector(rotCenterX, rotCenterY);
					targetPoint.Rotate(center, DegreesToRadians(rotAngle) );

					float mx = mirrorV ? -1.0f: 1.0f;
					float my = mirrorH ? -1.0f: 1.0f;

					particles[k].spr.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
				}
				else
				{
					particles[k].spr.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
				}
				particles[k].spr.SetDepth(particles[k].z + offsetZ);
				particles[k].spr.Alpha(particles[k].alpha);
				particles[k].spr.Rotate(particles[k].angle);
				particles[k].spr.SavePositions();

				// On empêche l'interpolation durant cette frame
				particles[k].spr.SavePositions();
			}
		}
	}
	
	particleBootTime -= 1.0f/30.0f;
	genTime += 1.0f/30.0f;
}

void ParticleGeneratorComponent::RotateLocal(float x, float y, float angle)
{
	rotCenterX = x;
	rotCenterY = y;
	rotAngle = angle;
}

void ParticleGeneratorComponent::MirrorLocal(bool v, bool h)
{
	oldMirrorV = mirrorV;
	oldMirrorH = mirrorH;

	mirrorV = v;
	mirrorH = h;
}