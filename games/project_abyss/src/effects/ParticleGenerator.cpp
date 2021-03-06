// project_abyss
// LittleSpace Studio 2012

#include "ParticleGenerator.h"
#include "../SimpleMaths.h"
#include <cmath>
#include "../physics/Vector.h"

ParticleGenerator::ParticleGenerator()
{
	numAdded = 0;

	Reset();
}

ParticleGenerator::ParticleGenerator(std::string filename)
{
	numAdded = 0;

	Reset();
	Load(filename);
}

ParticleGenerator::~ParticleGenerator()
{
	particles.swap(particles);
	particles.clear();
}

void ParticleGenerator::Reset()
{
	// Intialisation
	particleBootTime = -1.0f;
	isActive = true;
	genTime = 0.0f;
	movingParent_vx = movingParent_vy = movingParent_vz = 0.0f;
	priority = 0;
	rotCenterX = 0;
	rotCenterY = 0;
	rotAngle = 0;
	ignoreLightPipeline = true;
	offsetX = offsetY = offsetZ = 0.0f;
	particleCount = 0;
	emittedCount = 0;
	mirrorV = mirrorH = oldMirrorV = oldMirrorH = false;

	scene = NULL;
}

void ParticleGenerator::Load(std::string filename)
{
	Reset();

	// Chargement des param�tres
	params = (mk::ParticleGeneratorRessource*)mk::RessourceManager::getInstance()->LoadRessource(filename);
	
	// Param�tres pouvant �tre copi�s
	areaW = params->areaW;
	areaH = params->areaH;
	vx = params->vx;
	vy = params->vy;
	vz = params->vz;
}

void ParticleGenerator::SetArea(float w, float h)
{
	areaW = w;
	areaH = h;
}

void ParticleGenerator::Init(mk::Scene* sc)
{
	scene = sc;

	// Creating particles array
	if(params->maxParticles != -1)
	{
		if(particles.size() < params->maxParticles)
			particles.resize(params->maxParticles);

		for(int k = numAdded; k < params->maxParticles; k++) 
		{
			particles[k].active = false;
			
			// Sprite (Particle)
			particles[k].spr.Set3DMode(true);
			particles[k].spr.isCullingIgnored = true;
			scene->Add(&particles[k].spr);
			particles[k].spr.Hide();
			particles[k].spr.SetPriority(priority);
			particles[k].spr.ignoreLightPipeline = ignoreLightPipeline;

			// Sprite (Light)
			if(params->lightGenerator)
			{
				particles[k].light.Assign(params->lightGeneratorImg);
				particles[k].light.Set3DMode(true);
				particles[k].light.isCullingIgnored = true;
				scene->AddLight(&particles[k].light);
				particles[k].light.Hide();
				particles[k].light.SetPriority(priority);
			}

			numAdded++;
		}
	}
}

void ParticleGenerator::SetIgnoreLightPipeline(bool t)
{
	for(int k = 0; k < params->maxParticles; k++) 
	{
		particles[k].spr.ignoreLightPipeline = t;
	}
}

void ParticleGenerator::ChangePriority(int prio)
{
	for(int k = 0; k < params->maxParticles; k++) 
	{
		particles[k].spr.SetPriority(prio);
		if(params->lightGenerator)
			particles[k].light.SetPriority(prio);
	}
	priority = prio;
}

void ParticleGenerator::Update()
{
	stillAlive = false;

	for(int k = 0; k < params->maxParticles; k++) 
	{
		if(particles[k].active)
		{
			stillAlive = true;

			// Update des param�tres des particules
			particles[k].x += particles[k].vx;
            particles[k].y += particles[k].vy;
			particles[k].z += particles[k].vz;
			particles[k].alpha += particles[k].alphaFade;
			particles[k].angle += particles[k].angleVariation;
			particles[k].size += particles[k].sizeVariation;

			particles[k].life -= particles[k].fade;
			particles[k].vy += params->gravity;
			particles[k].spr.Show();
			if (particles[k].life <= 0.0f)
			{
				particles[k].active = false;
				particles[k].spr.Hide();
				if(params->lightGenerator)
					particles[k].light.Hide();
			}

			if(particles[k].size < 0.0f)
				particles[k].size = 0.0f;

			// D�placement du sprite dans le scene node
			particles[k].spr.SetSize((particles[k].spr.image->getImageWidth() / 32.0f) * particles[k].size, 
					(particles[k].spr.image->getImageHeight() / 32.0f) * particles[k].size);

			if(params->lightGenerator)
			{
				particles[k].light.SetSize((particles[k].light.image->getImageWidth() / 32.0f) * particles[k].size, 
						(particles[k].light.image->getImageHeight() / 32.0f) * particles[k].size);
			}

			if(!params->referencial)
			{
				NVector targetPoint = NVector(particles[k].x/32.0f, particles[k].y/32.0f);
				NVector center = NVector(rotCenterX, rotCenterY);
				targetPoint.Rotate(center, DegreesToRadians(rotAngle) );

				float mx = mirrorV ? -1.0f: 1.0f;
				float my = mirrorH ? -1.0f: 1.0f;

				particles[k].spr.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
				if(params->lightGenerator)
					particles[k].light.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
			}
			else
			{
				particles[k].spr.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
				if(params->lightGenerator)
					particles[k].light.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
			}

			// Changement de couleur
			if(params->colorKeys.size() != 0) {
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

			if(params->lightGenerator)
			{
				particles[k].light.SetDepth(particles[k].z + offsetZ);
				particles[k].light.Rotate(particles[k].angle);
				particles[k].light.Tint((unsigned char)(particles[k].alpha*255), (unsigned char)(particles[k].alpha*255), (unsigned char)(particles[k].alpha*255));
				particles[k].light.SavePositions();
			}

			// On emp�che l'interpolation durant cette frame
			if(oldMirrorV != mirrorV || oldMirrorH != mirrorH)
			{
				particles[k].spr.SavePositions();
				if(params->lightGenerator)
					particles[k].light.SavePositions();
			}
		}
		else
		{
			// Ajout d'une nouvelle particule
			if (particleBootTime <= 0.0f && isActive && (params->limitParticles == 0 || particleCount < params->limitParticles) )
			{
				emittedCount++;

				if(emittedCount == params->emitAmount)
				{
					particleBootTime = 1.0f/params->frequency;
					emittedCount = 0;
				}

				particleCount++;
				if(particleCount < params->limitParticles)
					stillAlive = true;

				particles[k].spr.Show();
				particles[k].spr.SetPriority(priority);

				if(params->lightGenerator)
				{
					particles[k].light.Show();
					particles[k].light.SetPriority(priority);
				}

				particles[k].life = params->initialLife;
				particles[k].fade = params->fade + SimpleMaths::Rand(params->fade_rmin, params->fade_rmax);
                particles[k].active = true;

				if(params->referencial == false)
				{
					particles[k].x = 0; 
					particles[k].y = 0;
					particles[k].z = 0;
				}
				else
				{
					particles[k].x = x; 
					particles[k].y = y;
					particles[k].z = z;
				}

				if(params->isAreaGenerator && params->areaType == mk::AREA_RECT)
				{
					particles[k].x += SimpleMaths::Rand(0.0f, areaW);
					particles[k].y += SimpleMaths::Rand(0.0f, areaH);
				}
				
				particles[k].z += SimpleMaths::Rand(params->areaZ_min, params->areaZ_max);

				particles[k].vx = vx + params->rangeFuncs[mk::PARAM_VEL_X]->func(genTime, params->vx_rmin, params->vx_rmax) + movingParent_vx;
				particles[k].vy = vy + params->rangeFuncs[mk::PARAM_VEL_Y]->func(genTime, params->vy_rmin, params->vy_rmax) + movingParent_vy;
				particles[k].vz = vz + params->rangeFuncs[mk::PARAM_VEL_Z]->func(genTime, params->vz_rmin,params-> vz_rmax) + movingParent_vz;
				
				if(!params->referencial == false)
				{
					float mx = mirrorV ? -1.0f: 1.0f;
					float my = mirrorH ? -1.0f: 1.0f;

					particles[k].vx = particles[k].vx * mx;
					particles[k].vy = particles[k].vy * my;
				}

				particles[k].alpha = params->initialAlpha;
				particles[k].alphaFade = params->alphaFade;
				particles[k].angle = params->alphaFade;
				particles[k].angleVariation = params->angleVariation;

				particles[k].size = params->initialSize;
				particles[k].sizeVariation = params->sizeVariation;

				particles[k].r = params->initialColorR;
				particles[k].g = params->initialColorG;
				particles[k].b = params->initialColorB;
				
				particles[k].spr.Assign(PickSprite() );

				particles[k].spr.SetSize((particles[k].spr.image->getImageWidth() / 32.0f) * particles[k].size, 
					(particles[k].spr.image->getImageHeight() / 32.0f) * particles[k].size);
				if(params->lightGenerator)
					particles[k].light.SetSize((particles[k].light.image->getImageWidth() / 32.0f) * particles[k].size, 
						(particles[k].light.image->getImageHeight() / 32.0f) * particles[k].size);

				if(!params->referencial)
				{
					NVector targetPoint = NVector(particles[k].x/32.0f, particles[k].y/32.0f);
					NVector center = NVector(rotCenterX, rotCenterY);
					targetPoint.Rotate(center, DegreesToRadians(rotAngle) );

					float mx = mirrorV ? -1.0f: 1.0f;
					float my = mirrorH ? -1.0f: 1.0f;

					particles[k].spr.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
					if(params->lightGenerator)
						particles[k].light.MoveTo(targetPoint.x*mx + offsetX, targetPoint.y*my + offsetY);
				}
				else
				{
					particles[k].spr.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
					if(params->lightGenerator)
						particles[k].light.MoveTo(particles[k].x/32.0f + offsetX, particles[k].y/32.0f + offsetY);
				}
				particles[k].spr.SetDepth(particles[k].z + offsetZ);
				particles[k].spr.Alpha(particles[k].alpha);
				particles[k].spr.Rotate(particles[k].angle);
				particles[k].spr.SavePositions();

				if(params->lightGenerator)
				{
					particles[k].light.SetDepth(particles[k].z + offsetZ);
					particles[k].light.Alpha(particles[k].alpha);
					particles[k].light.Rotate(particles[k].angle);
					particles[k].light.SavePositions();
				}

				// On emp�che l'interpolation durant cette frame
				particles[k].spr.SavePositions();
				particles[k].light.SavePositions();
			}
		}
	}
	
	// Desavtivation du g�n�rateur
	if(params->limitParticles != 0 && particleCount >= params->limitParticles && !stillAlive) 
	{
		isActive = false;
	}

	particleBootTime -= 1.0f/30.0f;
	genTime += 1.0f/30.0f;
}

void ParticleGenerator::RotateLocal(float px, float py, float angle)
{
	rotCenterX = px;
	rotCenterY = py;
	rotAngle = angle;
}

void ParticleGenerator::MirrorLocal(bool v, bool h)
{
	oldMirrorV = mirrorV;
	oldMirrorH = mirrorH;

	mirrorV = v;
	mirrorH = h;
}

void ParticleGenerator::GetColorInterp(float life, float *r, float *g, float *b)
{
	// On trouve les deux frames cl� � interpoler en fonction de life
	int prevFrame = 0;
	for(int i = 0; i < params->colorKeys.size(); i++)
	{
		if(i == params->colorKeys.size() - 1)
		{
			prevFrame = i;
			break;
		}
		else if(life > params->colorKeys[i].time && life < params->colorKeys[i+1].time)
		{
			prevFrame = i;
			break;
		}
	}

	int nextFrame = prevFrame + 1;
	if(nextFrame > params->colorKeys.size() - 1)
		nextFrame = params->colorKeys.size() - 1;

	// On r�alise l'interpolation des deux couleurs
	// Convertion des deux couleurs en HSL
	float h1, s1, l1, h2, s2, l2;
	RGB2HSL(params->colorKeys[prevFrame].r, params->colorKeys[prevFrame].g, params->colorKeys[prevFrame].b, &h1, &s1, &l1);
	RGB2HSL(params->colorKeys[nextFrame].r, params->colorKeys[nextFrame].g, params->colorKeys[nextFrame].b, &h2, &s2, &l2);

	// Interpolation
	float ith, its, itl;
	if(nextFrame != prevFrame)
	{
		ith = wrapf(Slerp2D(life - params->colorKeys[prevFrame].time, 0.0f, params->colorKeys[nextFrame].time - params->colorKeys[prevFrame].time, h1*PI*2, h2*PI*2)/(PI*2), 0, 1);
		its = Lerp(life - params->colorKeys[prevFrame].time, 0.0f, params->colorKeys[nextFrame].time - params->colorKeys[prevFrame].time, s1, s2);
		itl = Lerp(life - params->colorKeys[prevFrame].time, 0.0f, params->colorKeys[nextFrame].time - params->colorKeys[prevFrame].time, l1, l2);
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

void ParticleGenerator::ColorInterpolation(float life, float r1, float g1, float b1, float r2, float g2, float b2, float* tr, float* tg, float* tb)
{
	// On r�alise l'interpolation des deux couleurs
	// Convertion des deux couleurs en HSL
	float h1, s1, l1, h2, s2, l2;
	RGB2HSL(r1, g1, b1, &h1, &s1, &l1);
	RGB2HSL(r2, g2, b2, &h2, &s2, &l2);

	// Interpolation
	float ith, its, itl;

	ith = wrapf(Slerp2D(life, 0.0f, 1.0f, h1*PI*2, h2*PI*2)/(PI*2), 0, 1);
	its = Lerp(life, 0.0f, 1.0f, s1, s2);
	itl = Lerp(life, 0.0f, 1.0f, l1, l2);

	// ReConversion en RGB
	HSL2RGB(ith, its, itl, tr, tg, tb);
}
	
mk::Image* ParticleGenerator::PickSprite()
{
	int idOfImg = 0;

	// Optimisation pour les g�n�rateurs avec une seule image
	if(params->imgs.size() == 1)
		return params->imgs[0].img;

	float u = SimpleMaths::Rand(0.0f, 1.0f);

	float borneInf = 0;

	for(int i = 0; i < params->imgs.size(); i++)
	{
		if(u > borneInf && u < borneInf + params->imgs[i].frequency)
		{
			idOfImg = i;
			break;
		}

		borneInf += params->imgs[i].frequency;
	}

	//std::cout << idOfImg << std::endl;
	return params->imgs[idOfImg].img;
}

void ParticleGenerator::HSL2RGB(float h, float s, float l, float* outR, float* outG, float* outB)
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

void ParticleGenerator::RGB2HSL(float r, float g, float b, float* outH, float* outS, float* outL)
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