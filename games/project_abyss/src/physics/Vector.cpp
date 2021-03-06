/*
------------------------------------------------------------------
File: Vector.cpp
Started: 19/01/2004 21:43:13
  
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



#include "Vector.h"
#include "Matrix.h"

NVector NVector::operator * (const Matrix& M) const
{
	NVector T;
	T.x = x * M.e11 + y * M.e12;
	T.y = x * M.e21 + y * M.e22;
	return T;
}

NVector NVector::operator ^ (const Matrix& M) const
{
	NVector T;
	T.x = x * M.e11 + y * M.e21;
	T.y = x * M.e12 + y * M.e22;
	return T;
}

NVector& NVector::operator *=(const Matrix& M)
{
	NVector T = *this;
	x = T.x * M.e11 + T.y * M.e12;
	y = T.x * M.e21 + T.y * M.e22;
	return *this;
}

NVector& NVector::operator ^=(const Matrix& M)
{
	NVector T = *this;
	x = T.x * M.e11 + T.y * M.e21;
	y = T.x * M.e12 + T.y * M.e22;
	return *this;
}

bool FindRoots(float a, float b, float c, float& t0, float& t1)
{
	float d = b*b - (4.0f * a * c);

	if (d < 0.0f)
		return false;

	d = (float) sqrt(d);

	float one_over_two_a = 1.0f / (2.0f * a);

	t0 = (-b - d) * one_over_two_a;
	t1 = (-b + d) * one_over_two_a;

	if (t1 < t0)
	{
		float t = t0;
		t0 = t1;
		t1 = t;
	}
	return true;
}


bool RaySphereIntersection(const NVector& C, float r, const NVector& O, const NVector& D, float tmin, float tmax, float& t)
{
	float t0, t1;

	NVector H = (O - C);
	float  a = (D * D);
	float  b = (H * D) * 2.0f;
	float  c = (H * H) - r*r;

	if (!FindRoots(a, b, c, t0, t1))
		return false;

	if (t0 > tmax || t1 < tmin)
		return false;

	t = t0;

	if (t0 < tmin)
		t = t1;

	return true;
}


//--------------------------------------------------------
// Tools
//--------------------------------------------------------
void RenderSolidCircle(const NVector& xPos, float fRad)
{
	/*static int glList = -1;

	if (!glIsList(glList))
	{
		glList = glGenLists(1);

		glNewList(glList, GL_COMPILE_AND_EXECUTE);

		glBegin(GL_TRIANGLE_FAN);
		
		glVertex2f(0.0f, 0.0f);
		
		for(int i = 0; i < 16 + 2; i ++)
		{
			NVector P(cos(Pi() * (i / 8.0f)), sin(Pi() * (i / 8.0f)));

			glVertex2f(P.x, P.y);
		}

		glEnd();

		glEndList();
	}

	glPushMatrix();
	glTranslatef(xPos.x, xPos.y, 0.0f);
	glScalef(fRad, fRad, fRad);
	glCallList(glList);
	glPopMatrix();*/
}


void RenderCircle(const NVector& xPos, float fRad)
{
	/*static int glList = -1;

	if (!glIsList(glList))
	{
		glList = glGenLists(1);

		glNewList(glList, GL_COMPILE_AND_EXECUTE);

		glBegin(GL_LINE_LOOP);

		for(int i = 0; i < 16 + 1; i ++)
		{
			NVector P(cos(Pi() * (i / 8.0f)), sin(Pi() * (i / 8.0f)));

			glVertex2f(P.x, P.y);
		}

		glEnd();

		glEndList();
	}

	glPushMatrix();
	glTranslatef(xPos.x, xPos.y, 0.0f);
	glScalef(fRad, fRad, fRad);
	glCallList(glList);
	glPopMatrix();*/
}

void RenderArrow(const NVector& P, const NVector& D, float length, u_int uARGB)
{
	/*float fAngle = atan2(D.y, D.x);

	glPushMatrix();
	glTranslatef(P.x, P.y, 0.0f);
	glRotatef(RadiansToDegrees(fAngle), 0.0f, 0.0f, 1.0f);
	glScalef(length, length, 1.0f);

	glColor4ub(ARGB_R(uARGB), ARGB_G(uARGB), ARGB_B(uARGB), ARGB_A(uARGB));

	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.75f, 0.2f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.75f,-0.2f);
	glEnd();

	glPopMatrix();*/
}


