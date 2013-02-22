#ifndef OLI_POLYGON_H
#define OLI_POLYGON_H

#include "Vector.h"
/*
#include "Polygon.h"
*/

/*
------------------------------------------------------------------
File: Polygon.h
Started: 12/06/2004 14:37:12
  
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


class NVector;

class PolyColl
{
public:
	static NVector*	BuildBox	(int& iNumVertices, float width, float height);
	static NVector*	BuildBlob	(int iNumVertices, float radius);
	static void		Render		(const NVector& xOffset, float angle, u_int ARGBfill, u_int ARGBline, const NVector* axVertices, int iNumVertices);
	static bool		Collide		(const NVector* A, int Anum, const NVector& PA, const NVector& VA, const Matrix& OA,
								 const NVector* B, int Bnum, const NVector& PB, const NVector& VB, const Matrix& OB,
								 NVector& N, float& t);
};



#endif