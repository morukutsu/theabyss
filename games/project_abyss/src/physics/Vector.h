#ifndef OLI_NVector_H
#define OLI_NVector_H


/*
------------------------------------------------------------------
File: NVector.h
Started: 09/01/2004 22:20:40
  
$Header: $
$Revision: $
$Locker: $
$Date: $
  
Author: Olivier renault
------------------------------------------------------------------
Module: 
Description: 
------------------------------------------------------------------
$History: $
------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mkengine.h"

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

#define for if(0); else for

inline float sign(float x)
{
	return (x < 0.0f)? -1.0f : 1.0f;
}

inline float frand(float x=1.0f)
{
	return (rand() / (float) RAND_MAX) * x;
}

//===========================================================================
// NVectorS
//===========================================================================
class Matrix;

class NVector
{
public:
	float x,y;

	static const NVector& Blank() { static const NVector V(0, 0); return V; }
public:
	inline NVector(void)
	{}

	inline NVector(float Ix,float Iy)
	: x(Ix)
	, y(Iy)
	{}

	inline NVector &operator /=(const float Scalar)	{ x /= Scalar; y /= Scalar;		return *this; }

	inline NVector &operator *=(const float Scalar)	{ x *= Scalar; y *= Scalar;		return *this; }
	
	inline NVector &operator +=(const NVector &Other) { x += Other.x;	y += Other.y;	return *this; }

	inline NVector &operator -=(const NVector &Other) { x -= Other.x;	y -= Other.y;	return *this;	}

	inline float operator ^ (const NVector &V)	const	{	return (x * V.y) - (y * V.x); } // cross product

	inline float operator * (const NVector &V)	const	{	return (x*V.x) + (y*V.y); } // dot product

	inline NVector operator * (float  s)			const	{	return NVector(x*s, y*s); }
	
	inline NVector operator / (float  s)			const	{	return NVector(x/s, y/s); }
	
	inline NVector operator + (const NVector &V)	const	{	return NVector(x+V.x, y+V.y); }
		
	inline NVector operator - (const NVector &V)	const	{	return NVector(x-V.x, y-V.y); }

	friend NVector operator * (float k, const NVector& V) {	return NVector(V.x*k, V.y*k); }

	NVector operator * (const Matrix& M) const;
	
	NVector operator ^ (const Matrix& M) const;
	
	NVector& operator *=(const Matrix& M);
	
	NVector& operator ^=(const Matrix& M);

	inline NVector operator -(void) const { return NVector(-x, -y); }
	
	inline float Length(void) const { return (float) sqrt(x*x + y*y); }

	float Normalise(void) 
	{	
		float fLength = Length();	
		
		if (fLength == 0.0f) 
			return 0.0f; 
		
		(*this) *= (1.0f / fLength); 
	
		return fLength;	
	}

	NVector Direction(void) const
	{
		NVector Temp(*this);

		Temp.Normalise();

		return Temp;
	}

	float Angle(const NVector& xE)
	{
		float dot = (*this) * xE;
		float cross = (*this) ^ xE;
		
		// angle between segments
		float angle = (float) atan2(cross, dot);

		return angle;
	}

	NVector& Rotate(float angle)
	{
		float tx = x;
		x =  x * cos(angle) - y * sin(angle);
		y = tx * sin(angle) + y * cos(angle);

		return *this;
	}

	NVector& Rotate(const NVector& xCentre, float fAngle)
	{
		NVector D = *this - xCentre;
		D.Rotate(fAngle);

		*this = xCentre + D;

		return *this;
	}

	void Clamp(const NVector& min, const NVector& max)
	{
		x = (x < min.x)? min.x : (x > max.x)? max.x : x;
		y = (y < min.y)? min.y : (y > max.y)? max.y : y;
	}

	void Randomise(const NVector& xMin, const NVector& xMax)
	{
		x = frand(xMax.x - xMin.x) + xMin.x;
		y = frand(xMax.y - xMin.y) + xMin.y;
	}

	void Render(void) const
	{
		/*glColor3f(1.0f, 1.0f, 0.1f);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();*/
	}
};

#define ARGB_A(u) (((u)>>24) & 0x000000FF)
#define ARGB_R(u) (((u)>>16) & 0x000000FF)
#define ARGB_G(u) (((u)>> 8) & 0x000000FF)
#define ARGB_B(u) (((u)>> 0) & 0x000000FF)

extern void RenderArrow			 (const NVector& P, const NVector& D, float length, u_int uARGB);
extern void	RenderSolidCircle	 (const NVector& xPos, float fRad);
extern void	RenderCircle		 (const NVector& xPos, float fRad);
extern bool FindRoots			 (float a, float b, float c, float& t0, float& t1);
extern bool RaySphereIntersection(const NVector& C, float r, const NVector& O, const NVector& D, float tmin, float tmax, float& t);



#endif