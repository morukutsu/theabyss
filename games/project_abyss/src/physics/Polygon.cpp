
#include "Vector.h"
#include "Matrix.h"
#include "Polygon.h"

#include <iostream>

NVector* PolyColl::BuildBox(int& iNumVertices, float width, float height)
{
	NVector* axVertices = new NVector[4];
	
	axVertices[0] = NVector(-width/2, -height/2);
	axVertices[1] = NVector( width/2, -height/2);
	axVertices[2] = NVector( width/2,  height/2);
	axVertices[3] = NVector(-width/2,  height/2);

	iNumVertices = 4;
	return axVertices;
}

NVector* PolyColl::BuildBlob(int iNumVertices, float radius)
{
	NVector* axVertices = new NVector[iNumVertices];

	float a  = Pi() / iNumVertices;
	float da = TwoPi() / iNumVertices;

	for(int i = 0; i < iNumVertices; i ++)
	{
		a += da;

		axVertices[i] = NVector(cos(a) * radius, sin(a) * radius);
	}
	return axVertices;
}

void PolyColl::Render(const NVector& xOffset, float angle, u_int ARGBfill, u_int ARGBline, const NVector* axVertices, int iNumVertices)
{
	/*if (!axVertices) return;

	glPushMatrix();

	glTranslatef(xOffset.x, xOffset.y, 0.0f);

	glRotatef(RadiansToDegrees(angle), 0, 0, -1);

	glColor4ub(ARGB_R(ARGBfill), ARGB_G(ARGBfill), ARGB_B(ARGBfill), ARGB_A(ARGBfill));

	glBegin(GL_TRIANGLE_FAN);

	for(int i = 0; i < iNumVertices; i ++)
	{
		glVertex2f(axVertices[i].x, axVertices[i].y);
	}
	glVertex2f(axVertices[0].x, axVertices[0].y);
	glEnd();

	if (ARGBline != 0)
	{
		glColor4ub(ARGB_R(ARGBline), ARGB_G(ARGBline), ARGB_B(ARGBline), ARGB_A(ARGBline));
		
		glBegin(GL_LINE_LOOP);

		for(int i = 0; i < iNumVertices; i ++)
		{
			glVertex2f(axVertices[i].x, axVertices[i].y);
		}
		glVertex2f(axVertices[0].x, axVertices[0].y);
		glEnd();
	}
	glPopMatrix();*/
}	


void GetInterval(const NVector *axVertices, int iNumVertices, 
				 const NVector& xAxis, float& min, float& max);

bool FindMTD(NVector* xAxis, float* taxis, int iNumAxes, NVector& N, float& t);

bool IntervalIntersect(const NVector* A, int Anum,
					   const NVector* B, int Bnum,
					   const NVector& xAxis, 
					   const NVector& xOffset, const NVector& xVel, const Matrix& xOrient,
					   float& taxis, float tmax);

bool PolyColl::Collide(const NVector* A, int Anum, const NVector& PA, const NVector& VA, const Matrix& OA,
					   const NVector* B, int Bnum, const NVector& PB, const NVector& VB, const Matrix& OB,
					   NVector& N, float& t)
{
	if (!A || !B) return false;

	Matrix xOrient = OA ^ OB;
	NVector xOffset = (PA - PB) ^ OB;
	NVector xVel    = (VA - VB) ^ OB;
	
	// All the separation axes
	NVector xAxis[64]; // note : a maximum of 32 vertices per poly is supported
	float  taxis[64];
	int    iNumAxes=0;

	float fVel2 = xVel * xVel;

	if (fVel2 > 0.00001f)
	{
		xAxis[iNumAxes] = NVector(-xVel.y, xVel.x);

		if (!IntervalIntersect(	A, Anum, 
								B, Bnum, 
								xAxis[iNumAxes], 
								xOffset, xVel, xOrient,
								taxis[iNumAxes], t))
		{
			return false;
		}
		iNumAxes++;
	}

	// test separation axes of A
	for(int j = Anum-1, i = 0; i < Anum; j = i, i ++)
	{
		NVector E0 = A[j];
		NVector E1 = A[i];
		NVector E  = E1 - E0;
		xAxis[iNumAxes] = NVector(-E.y, E.x) * xOrient;
		
		if (!IntervalIntersect(	A, Anum, 
								B, Bnum, 
								xAxis[iNumAxes], 
								xOffset, xVel, xOrient,
								taxis[iNumAxes], t))
		{
			return false;
		}

		iNumAxes++;
	}

	// test separation axes of B
	for(int j = Bnum-1, i = 0; i < Bnum; j = i, i ++)
	{
		NVector E0 = B[j];
		NVector E1 = B[i];
		NVector E  = E1 - E0;
		xAxis[iNumAxes] = NVector(-E.y, E.x);

		if (!IntervalIntersect(	A, Anum, 
								B, Bnum, 
								xAxis[iNumAxes], 
								xOffset, xVel, xOrient,
								taxis[iNumAxes], t))
		{
			return false;
		}

		iNumAxes++;
	}

	// special case for segments
	if (Bnum == 2)
	{
		NVector E  = B[1] - B[0];
		xAxis[iNumAxes] = E;
		
		if (!IntervalIntersect(	A, Anum, 
								B, Bnum, 
								xAxis[iNumAxes], 
								xOffset, xVel, xOrient,
								taxis[iNumAxes], t))
		{
			return false;
		}

		iNumAxes++;
	}

	// special case for segments
	if (Anum == 2)
	{
		NVector E  = A[1] - A[0];
		xAxis[iNumAxes] = E * xOrient;
		
		if (!IntervalIntersect(	A, Anum, 
								B, Bnum, 
								xAxis[iNumAxes], 
								xOffset, xVel, xOrient,
								taxis[iNumAxes], t))
		{
			return false;
		}

		iNumAxes++;
	}

	if (!FindMTD(xAxis, taxis, iNumAxes, N, t))
		return false;

	// make sure the polygons gets pushed away from each other.
	if (N * xOffset < 0.0f)
		N = -N;

	N *= OB;

	return true;
}

// calculate the projection range of a polygon along an axis
void GetInterval(const NVector *axVertices, int iNumVertices, const NVector& xAxis, float& min, float& max)
{
	min = max = (axVertices[0] * xAxis);

	for(int i = 1; i < iNumVertices; i ++)
	{
		float d = (axVertices[i] * xAxis);
		if (d < min) min = d; else if (d > max) max = d;
	}
}

bool IntervalIntersect(const NVector* A, int Anum,
					   const NVector* B, int Bnum,
					   const NVector& xAxis, 
					   const NVector& xOffset, const NVector& xVel, const Matrix& xOrient,
					   float& taxis, float tmax)
{
	float min0, max0;
	float min1, max1;
	GetInterval(A, Anum, xAxis ^ xOrient, min0, max0);
	GetInterval(B, Bnum, xAxis, min1, max1);

	float h = xOffset * xAxis;
	min0 += h;
	max0 += h;

	float d0 = min0 - max1; // if overlapped, do < 0
	float d1 = min1 - max0; // if overlapped, d1 > 0

	// separated, test dynamic intervals
	if (d0 > 0.0f || d1 > 0.0f) 
	{
		float v = xVel * xAxis;

		// small velocity, so only the overlap test will be relevant. 
		if (fabs(v) < 0.0000001f)
			return false;

		float t0 =-d0 / v; // time of impact to d0 reaches 0
		float t1 = d1 / v; // time of impact to d0 reaches 1

		if (t0 > t1) { float temp = t0; t0 = t1; t1 = temp; }
		taxis  = (t0 > 0.0f)? t0 : t1;

		if (taxis < 0.0f || taxis > tmax)
			return false;

		return true;
	}
	else
	{
		// overlap. get the interval, as a the smallest of |d0| and |d1|
		// return negative number to mark it as an overlap
		taxis = (d0 > d1)? d0 : d1;
		return true;
	}
}
bool FindMTD(NVector* xAxis, float* taxis, int iNumAxes, NVector& N, float& t)
{
	// find collision first
	int mini = -1;
	t = 0.0f;
	for(int i = 0; i < iNumAxes; i ++)
	{	
		if (taxis[i] > 0)
		{
			if (taxis[i] > t)
			{
				mini = i;
				t = taxis[i];
				N = xAxis[i];
				N.Normalise();
			}
		}
	}

	// found one
	if (mini != -1)
		return true; 

	// nope, find overlaps
	mini = -1;
	for(int i = 0; i < iNumAxes; i ++)
	{
		float n = xAxis[i].Normalise();
		taxis[i] /= n;

		if (taxis[i] > t || mini == -1)
		{
			mini = i;
			t = taxis[i];
			N = xAxis[i];
		}
	}

	if (mini == -1)
		printf("Error\n");

	return (mini != -1);
}

