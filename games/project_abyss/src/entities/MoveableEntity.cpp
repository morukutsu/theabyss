// project_abyss
// LittleSpace Studio 2012

#include "MoveableEntity.h"

MoveableEntity::MoveableEntity()
{

}

MoveableEntity::~MoveableEntity()
{

}

void MoveableEntity::Init()
{
	InitComponents();

	mVel = NVector(0, 0);
}

void MoveableEntity::Resolve()
{

}

void MoveableEntity::Cleanup()
{

}

void MoveableEntity::Update()
{
	UpdateComponents();
}

void MoveableEntity::Receive(int message, void* data)
{

}  