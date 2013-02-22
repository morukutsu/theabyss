// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "SimpleMaths.h"
#include <cstdlib>
#include <math.h>
#include "mkengine.h"

double SimpleMaths::InterpolateCosinus(double y1, double y2, double mu)
{
	double mu2;
 
	mu2 = (1-cos(mu*PI))/2;
       return (y1+mu2*(y2-y1));
}

double SimpleMaths::InterpolationLineaire(double v1, double v2, double mu)
{
	return (v1+mu*(v2-v1));
}

double SimpleMaths::InterpolationCubique(double y0,double y1,double y2,double y3,double mu)
{
    double a0,a1,a2,a3,mu2;
 
    mu2 = mu*mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;
 
    return (a0*mu*mu2+a1*mu2+a2*mu+a3);
}

int SimpleMaths::Rand(int min, int max)
{
	return (int) (min + ((float) rand() / RAND_MAX * (max - min )));
}

float SimpleMaths::Rand(float min, float max)
{
	return (min + ((float) rand() / RAND_MAX * (max - min )));
}

float SimpleMaths::GetAngle2Points(float x1, float y1, float x2, float y2)
{
	float i = x1 - x2;
	float j = -y1 + y2;

	if(i == 0 && j == 0)
		return 0.0f;
	else
		return atan2(i, j)*180/3.14f;
}

float SimpleMaths::GetAngle2PointsRad(float x1, float y1, float x2, float y2)
{
	float i = x1 - x2;
	float j = -y1 + y2;

	if(i == 0 && j == 0)
		return 0.0f;
	else
		return atan2(i, j);
}