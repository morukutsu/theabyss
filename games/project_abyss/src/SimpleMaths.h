// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_SIMPLEMATHS
#define MK_SIMPLEMATHS

class SimpleMaths
{
public:
	static double InterpolateCosinus(double y1, double y2, double mu);
	static double InterpolationLineaire(double v1, double v2, double mu);
	static double InterpolationCubique(double y0,double y1,double y2,double y3,double mu);
	static int    Rand(int min, int max);
	static float  Rand(float min, float max);
	static float  GetAngle2Points(float x1, float y1, float x2, float y2);
	static float  GetAngle2PointsRad(float x1, float y1, float x2, float y2);
};

#endif