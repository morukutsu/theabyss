/*
------------------------------------------------------------------
File: Body.cpp
Started: 08/02/2004 12:05:02
  
$Header: $
$Revision: $
$Locker: $
$Date: $
  
Author: Olivier renault
------------------------------------------------------------------
Module: 
Description: 
----------------------------*--------------------------------------
$History: $
------------------------------------------------------------------
*/

#include "Body.h"
#include "Entities/Entity.h"

//extern float  dbg_world_size;

CBody::CBody()
{
	m_axVertices = 0;
	m_iNumVertices = 0;

	s_fFriction	= 0.0f;
	s_fRestitution = 0.1f;
	s_fGlue		= 0.01f;

	NVector xPos    = NVector(0, 0);
	float  fSize   = frand(0) * 0.02f + 0 * 0.04f;
	float  fDensity= (frand(1.0f) < 0.7f)? 0.0f : 1.0f;
	
	if (fDensity < 0.0001f) fDensity = 0.0f;
	
	float fMass = fDensity * fSize * fSize;

	if (fMass < 0.0001f) fMass = 0.0f;
	
	int iNumVertices = rand() % 6 + 2;
	NVector* axVertices = PolyColl::BuildBlob(iNumVertices, fSize);

	Initialise(xPos, fMass, axVertices, iNumVertices);
	isCollision = isSensor = toDelete = isSleeping = isGravity = ignoreCollisions = false;
}

void CBody::Shutdown()
{
	if (m_axVertices)
		delete[] m_axVertices;

	m_axVertices = NULL;
	m_iNumVertices = 0;
}
void CBody::Initialise(const NVector& xPosition, float fMass, NVector* axVertices, int iNumVertices)
{
	//Shutdown();
	m_axVertices = NULL;
	m_iNumVertices = 0;

	m_xDisplacement	= NVector(0, 0);
	m_xPosition		= xPosition;
	m_fMass			= fMass;
	m_fInvMass		= (fMass > 0.0000001f)? 1.0f / fMass : 0.0f;

	m_axVertices   = axVertices;
	m_iNumVertices = iNumVertices;

	//m_fAngle = (iNumVertices != 4)? frand(3.0f) : 0.0f;
	//m_fAngVelocity = frand(3.0f) + 1.0f;
	m_fAngle = 0;
	m_fAngVelocity = 0;
	m_xOrientation = Matrix(m_fAngle);

	isCollision = isSensor = toDelete = isSleeping = isGravity = ignoreCollisions = false;
}

CBody::CBody(const NVector& xPosition, float fMass, float width, float height)
{
	m_axVertices = 0;
	m_iNumVertices = 0;

	int iNumVertices = rand() % 8 + 3;
	NVector* axVertices = PolyColl::BuildBox(iNumVertices, width, height);
	Initialise(xPosition, fMass, axVertices, iNumVertices);
	isCollision = isSensor = toDelete = isSleeping = isGravity = ignoreCollisions = false;
}

CBody::~CBody()
{
	Shutdown();
}

void CBody::AddImpulse(const NVector& F, float dt)
{
	if (IsUnmovable()) return;

	m_xDisplacement += F * (m_fInvMass * dt*dt);
}

void CBody::ComputeGravity(const NVector& F, float dt)
{
	if (IsUnmovable()) return;
	
	//Limite
	if(m_xG.Length() <= 10.0f)
		m_xG += F * (m_fInvMass * dt*dt);
}

void CBody::Update(float dt)
{
	if (IsUnmovable())
	{
		m_xDisplacement = NVector(0, 0);
		return;
	}
	//m_xDisplacement += m_xG;
	m_xPrevposition = m_xPosition;
	m_xPosition += m_xDisplacement;

	m_fAngle      += m_fAngVelocity * dt;
	m_xOrientation = Matrix(m_fAngle);

	wrapf(m_fAngle, 0, TwoPi());
}

void CBody::Render() const
{	
	u_int uFill = (IsUnmovable())? 0x808080FF : 0x8080FF80;
	u_int uLine = (IsUnmovable())? 0xFF8080FF : 0xFF008000;

	PolyColl::Render(m_xPosition, m_fAngle, uFill, uLine, m_axVertices, m_iNumVertices);
}

bool CBody::Collide(CBody& xBody)
{
	if(ignoreCollisions || xBody.ignoreCollisions)
		return false;

	if(isSensor == false && xBody.isSensor == false)
	{
		if (IsUnmovable() && xBody.IsUnmovable())	
			return false;
	}
	//isCollisionWithPlanet = false;
	//isCollision = false;
	//xBody.isCollision = false;

    NVector xMTD;
    float  t = 1.0f;
	NVector N;

	const NVector* A  = m_axVertices;
	int Anum         = m_iNumVertices;
	const NVector& PA = m_xPosition;
	const NVector& VA = m_xDisplacement;
	Matrix OA        = m_xOrientation;//Matrix::Identity();
	
	const NVector* B  = xBody.m_axVertices;
	int Bnum         = xBody.m_iNumVertices;
	const NVector& PB = xBody.m_xPosition;
	const NVector& VB = xBody.m_xDisplacement;
	Matrix OB        = xBody.m_xOrientation;//Matrix::Identity();

	if (PolyColl::Collide(A, Anum, PA, VA, OA, B, Bnum, PB, VB, OB, N, t))
	{
		if(isSensor == false && xBody.isSensor == false)
		{
			if (t < 0.0f)
			{
				ProcessOverlap(xBody, N * -t);
			}
			else
			{
				ProcessCollision(xBody, N, t);
			}
		}
		
		return true;
	}
	

	return false;
}

// two objects collided at time t. stop them at that time
void CBody::ProcessCollision(CBody& xBody, const NVector& N, float t)
{
	NVector D = m_xDisplacement - xBody.m_xDisplacement;

	float n  = D * N;
	
	NVector Dn = N * n;
	NVector Dt = D - Dn;
	
	if (n > 0.0f) Dn = NVector(0, 0);

	float dt  = Dt * Dt;
	float CoF = sqrtf(s_fFriction * xBody.s_fFriction);
	
	if (dt < s_fGlue*s_fGlue) CoF = 1.01f;

	D = -(1.0f + (s_fRestitution + xBody.s_fRestitution) ) * Dn - (CoF) * Dt;

	float m0 =       GetInvMass();
	float m1 = xBody.GetInvMass();
	float m  = m0 + m1;
	float r0 = m0 / m;
	float r1 = m1 / m;
	
		  m_xDisplacement += D * r0;
	xBody.m_xDisplacement -= D * r1;
	m_xG.x = 0;
	m_xG.y = 0; 
}

// two objects overlapped. push them away from each other
void CBody::ProcessOverlap(CBody& xBody, const NVector& xMTD)
{
	if (IsUnmovable())
	{
		xBody.m_xPosition -= xMTD;
	}
	else if (xBody.IsUnmovable())
	{
		m_xPosition += xMTD;
	}
	else
	{
			  m_xPosition += xMTD * 0.5f;
		xBody.m_xPosition -= xMTD * 0.5f;
	}
	
	NVector N = xMTD;
	N.Normalise();
	ProcessCollision(xBody, N, 0.0f);
}

void CBody::AddCollisionWithBody(CBody* b) 
{ 
	cbodies.push_back(b);
}

void CBody::ClearCollisionBodies()
{
	cbodies.clear();
}