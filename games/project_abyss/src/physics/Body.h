#ifndef OLI_BODY_H
#define OLI_BODY_H


/*
#include "Body.h"
*/

/*
------------------------------------------------------------------
File: Body.h
Started: 07/02/2004 20:55:15
  
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
#include "Polygon.h"
#include <vector>

 //Bodies
#include "BodyDef.h"

class CBody
{
public:
	CBody();
	
	CBody(const NVector& xPosition, float fMass, float width, float height);

	void Initialise(const NVector& xPosition, float fMass, NVector* axVertices, int iNumVertices);

	void Shutdown();
	
	~CBody();

	void AddImpulse(const NVector& F, float dt);
	
	bool Collide(CBody& Body);
	
	void Update(float dt);

	void Render() const;

	bool IsUnmovable() const { return (m_fMass < 0.0001f); }
	bool GetCollision() { return isCollision; };
	NVector& GetPosition		() { return m_xPosition; }
	NVector& GetPrevPosition		() { return m_xPrevposition; }
	NVector& GetDisplacement () { return m_xDisplacement; }
	float&  GetAngVelocity  () { return m_fAngVelocity; }
	float&  GetMass			() { return m_fMass; }
	float&  GetInvMass		() { return m_fInvMass; }
		
	const NVector& GetPosition	() const { return m_xPosition; }
	const NVector& GetDisplacement() const { return m_xDisplacement; }
	float GetMass				() const { return m_fMass; }
	float GetInvMass			() const { return m_fInvMass; }
	
public:
	float GetFriction   () { return s_fFriction; }
	float GetRestitution() { return s_fRestitution; }
	float GetGlue	   () { return s_fGlue; }

	void SetFriction   (float fCoF) { s_fFriction    = fCoF; }
	void SetRestitution(float fCoR) { s_fRestitution = fCoR; }
	void SetGlue		 (float fCoS) { s_fGlue  = fCoS; }

	void SetLinearVelocity		 (const NVector vel) { m_xDisplacement = vel; }
	void SetPosition		 (const NVector pos ) { m_xPosition = pos; }
	void SetOrientation(float angle) { m_fAngle = angle; m_xOrientation = Matrix(m_fAngle); }
	void ComputeGravity(const NVector& G, float dt);
	void ToggleSensor(bool s) { isSensor = s; };

	void AddCollisionWithBody(CBody* b);
	void ClearCollisionBodies();
	std::vector<CBody*>& GetCollisionsBodies() { return cbodies; } ;

	bool toDelete, isCollision, isSensor;
	int w, h;

	// Pour identifier un body
	int bodytype, bodykind;

	NVector* m_axVertices;
	int     m_iNumVertices;
	NVector m_center;
	NVector m_vg;
	NVector m_charImpulse;
	NVector	m_xDisplacement;
	Matrix m_xOrientation;
	float m_fAngle;
	float	m_fMass;
	float	m_fInvMass;

	std::vector <CBody*> cbodies;
	bool isSleeping;

private:
	float s_fFriction;
	float s_fRestitution;
	float s_fGlue;

protected:
	void ProcessCollision(CBody& xBody, const NVector& N, float t);
	void ProcessOverlap  (CBody& xBody, const NVector& MTD);

	NVector m_xG;
	NVector	m_xPosition, m_xPrevposition;
	
	float m_fAngVelocity;
	
};
#endif