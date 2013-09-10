// project_abyss
// LittleSpace Studio 2012

#include "ParticleGeneratorComponent.h"
#include "../entities/Entity.h"
#include "../SimpleMaths.h"
#include <cmath>

RangeFunc* createFunc(std::string name)
{
	if(name == "sin")
		return new RangeFuncSin();
	else if(name == "random")
		return new RangeFuncRandom();
	else
		return NULL;
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
	ignoreLightPipeline = true;
	isAreaGenerator = false;
	areaZ_min = areaZ_max = 0.0f;

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

		elem = elem->NextSiblingElement();
	}
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

void ParticleGeneratorComponent::SetArea(float w, float h)
{
	areaW = w;
	areaH = h;
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
			//particles[k].spr.Assign(img);
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
			particles[k].spr.SetSize((particles[k].spr.image->getImageWidth() / 32.0f) * particles[k].size, 
					(particles[k].spr.image->getImageHeight() / 32.0f) * particles[k].size);

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

			// Changement de couleur
			if(colorKeys.size() != 0) {
				GetColorInterp(1.0f - particles[k].life, &particles[k].r, &particles[k].g, &particles[k].b);
				/*particles[k].r = 0;
				particles[k].g = 0;
				particles[k].b = 0;
				std::cout << "plop" << std::endl;*/
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

				if(isAreaGenerator && areaType == AREA_RECT)
				{
					particles[k].x += SimpleMaths::Rand(0.0f, areaW);
					particles[k].y += SimpleMaths::Rand(0.0f, areaH);
				}
				
				particles[k].z += SimpleMaths::Rand(areaZ_min, areaZ_max);

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
				
				particles[k].spr.Assign(PickSprite() );

				particles[k].spr.SetSize((particles[k].spr.image->getImageWidth() / 32.0f) * particles[k].size, 
					(particles[k].spr.image->getImageHeight() / 32.0f) * particles[k].size);

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

void ParticleGeneratorComponent::GetColorInterp(float life, float *r, float *g, float *b)
{
	// On trouve les deux frames clé à interpoler en fonction de life
	int prevFrame = 0;
	for(int i = 0; i < colorKeys.size(); i++)
	{
		if(i == colorKeys.size() - 1)
		{
			prevFrame = i;
			break;
		}
		else if(life > colorKeys[i].time && life < colorKeys[i+1].time)
		{
			prevFrame = i;
			break;
		}
	}

	int nextFrame = prevFrame + 1;
	if(nextFrame > colorKeys.size() - 1)
		nextFrame = colorKeys.size() - 1;

	// On réalise l'interpolation des deux couleurs
	// Convertion des deux couleurs en HSL
	float h1, s1, l1, h2, s2, l2;
	RGB2HSL(colorKeys[prevFrame].r, colorKeys[prevFrame].g, colorKeys[prevFrame].b, &h1, &s1, &l1);
	RGB2HSL(colorKeys[nextFrame].r, colorKeys[nextFrame].g, colorKeys[nextFrame].b, &h2, &s2, &l2);

	// Interpolation
	float ith, its, itl;
	if(nextFrame != prevFrame)
	{
		ith = wrapf(Slerp2D(life - colorKeys[prevFrame].time, 0.0f, colorKeys[nextFrame].time - colorKeys[prevFrame].time, h1*PI*2, h2*PI*2)/(PI*2), 0, 1);
		its = Lerp(life - colorKeys[prevFrame].time, 0.0f, colorKeys[nextFrame].time - colorKeys[prevFrame].time, s1, s2);
		itl = Lerp(life - colorKeys[prevFrame].time, 0.0f, colorKeys[nextFrame].time - colorKeys[prevFrame].time, l1, l2);
	}
	else
	{
		ith = h1;
		its = s1;
		itl = l1;
	}

	// ReConversion en RGB
	HSL2RGB(ith, its, itl, r, g, b);

	//std::cout << "LIFE " << life << "(" << prevFrame << ") h1 : " << h1 << ", (" << nextFrame << ") h2 : " << h2 << ", interp : " << ith << std::endl;
}

mk::Image* ParticleGeneratorComponent::PickSprite()
{
	int idOfImg = 0;

	// Optimisation pour les générateurs avec une seule image
	if(imgs.size() == 1)
		return imgs[0].img;

	float u = SimpleMaths::Rand(0.0f, 1.0f);

	float borneInf = 0;

	for(int i = 0; i < imgs.size(); i++)
	{
		if(u > borneInf && u < borneInf + imgs[i].frequency)
		{
			idOfImg = i;
			break;
		}

		borneInf += imgs[i].frequency;
	}

	//std::cout << idOfImg << std::endl;
	return imgs[idOfImg].img;
}

void ParticleGeneratorComponent::HSL2RGB(float h, float s, float l, float* outR, float* outG, float* outB)
{
	float			temp1,
  temp2;
	float			temp[3];
	int				i;
	
	// Check for saturation. If there isn't any just return the luminance value for each, which results in gray.
	if(s == 0.0) {
		if(outR)
			*outR = l;
		if(outG)
			*outG = l;
		if(outB)
			*outB = l;
		return;
	}
	
	// Test for luminance and compute temporary values based on luminance and saturation 
	if(l < 0.5)
		temp2 = l * (1.0 + s);
	else
		temp2 = l + s - l * s;
  temp1 = 2.0 * l - temp2;
	
	// Compute intermediate values based on hue
	temp[0] = h + 1.0 / 3.0;
	temp[1] = h;
	temp[2] = h - 1.0 / 3.0;
  
	for(i = 0; i < 3; ++i) {
		
		// Adjust the range
		if(temp[i] < 0.0)
			temp[i] += 1.0;
		if(temp[i] > 1.0)
			temp[i] -= 1.0;
		
		
		if(6.0 * temp[i] < 1.0)
			temp[i] = temp1 + (temp2 - temp1) * 6.0 * temp[i];
		else {
			if(2.0 * temp[i] < 1.0)
				temp[i] = temp2;
			else {
				if(3.0 * temp[i] < 2.0)
					temp[i] = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - temp[i]) * 6.0;
				else
					temp[i] = temp1;
			}
		}
	}
	
	// Assign temporary values to R, G, B
	if(outR)
		*outR = temp[0];
	if(outG)
		*outG = temp[1];
	if(outB)
		*outB = temp[2];
}

void ParticleGeneratorComponent::RGB2HSL(float r, float g, float b, float* outH, float* outS, float* outL)
{
  float h,s, l, v, m, vm, r2, g2, b2;
  
  h = 0;
  s = 0;
  l = 0;
  
  v = std::max(r, g);
  v = std::max(v, b);
  m = std::min(r, g);
  m = std::min(m, b);
  
  l = (m+v)/2.0f;
  
  if (l <= 0.0){
    if(outH)
			*outH = h;
		if(outS)
			*outS = s;
		if(outL)
			*outL = l;
    return;
  }
  
  vm = v - m;
  s = vm;
  
  if (s > 0.0f){
    s/= (l <= 0.5f) ? (v + m) : (2.0 - v - m); 
  }else{
    if(outH)
			*outH = h;
		if(outS)
			*outS = s;
		if(outL)
			*outL = l;
    return;
  }
  
  r2 = (v - r)/vm;
  g2 = (v - g)/vm;
  b2 = (v - b)/vm;
  
  if (r == v){
    h = (g == m ? 5.0f + b2 : 1.0f - g2);
  }else if (g == v){
    h = (b == m ? 1.0f + r2 : 3.0 - b2);
  }else{
    h = (r == m ? 3.0f + g2 : 5.0f - r2);
  }
  
  h/=6.0f;
  
  if(outH)
    *outH = h;
  if(outS)
    *outS = s;
  if(outL)
    *outL = l;
  
}