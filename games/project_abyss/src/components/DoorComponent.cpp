// project_abyss
// LittleSpace Studio 2012

#include "DoorComponent.h"
#include "../physics/Body.h"
#include "../physics/Vector.h"
#include "../physics/Polygon.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

DoorComponent::DoorComponent(std::string _sens, float w, float h, std::string _target_map, std::string _target_door)
{
	if(_sens == "left")
	{
		sens = DOOR_LEFT;
	}
	else if(_sens == "right") 
	{
		sens = DOOR_RIGHT;
	}
	else if(_sens == "up") 
	{
		sens = DOOR_UP;
	}
	else if(_sens == "down") 
	{
		sens = DOOR_DOWN;
	}

	sensor = new CBody();
	width = w;
	height = h;
	target_map = _target_map;
	target_door = _target_door;

	state = DOOR_STATE_OPENED;
}

void DoorComponent::Init()
{
	// Création du body de la taille de la porte
	int numVerts = 4;
	NVector* points = PolyColl::BuildBox(numVerts, width, height);
	sensor->Initialise(parent->mPos + NVector(width/2, height/2), 0.0f, points, numVerts);
	sensor->isSensor = true;
	sensor->bodytype = BODY_FULL;

	parent->GetEntityManager()->GetGameMap()->AddBody(sensor);

	// Setting du bon sens de la porte quand on sort d'une porte depuis une autre map
	if(parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] == 1)
	{
		if(parent->mName == parent->GetEntityManager()->GetGameMap()->mSwitchMapDoorIdf)
		{
			// Sens opposé à la porte
			int oppositeSens;
			switch(sens)
			{
			case DOOR_LEFT:
				oppositeSens = DOOR_RIGHT;
				break;
			case DOOR_RIGHT:
				oppositeSens = DOOR_LEFT;
				break;
			case DOOR_UP:
				oppositeSens = DOOR_DOWN;
				break;
			case DOOR_DOWN:
				oppositeSens = DOOR_UP;
				break;
			};

			parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_DOOR_ORIENTATION] = oppositeSens;
		}
	}
}

void DoorComponent::Receive(int message, void* data)
{

}

void DoorComponent::Update()
{
	// Check des collisions avec la porte, ssi on ne traverse pas une porte
	if(parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_GOING_TROUGH_DOOR] == 0
		&& parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] == 0)
	{
		std::vector<CBody*> c = sensor->GetCollisionsBodies();
		for(std::vector<CBody*>::iterator it = c.begin(); it != c.end(); it++)
		{
			// Collision avec un héros
			if((*it)->bodytype == BODY_HERO)
			{
				switch(state)
				{
				case DOOR_STATE_OPENED:
					state = DOOR_STATE_SCROLLING;

					// On prévient le manager que le héros passe une porte
					MsgDataChangeStateVar data;
					data.idf = C_STATE_GOING_TROUGH_DOOR;
					data.value = 1;
					parent->SendToManager(MSG_CHANGE_STATE_VAR, (void*)&data);

					// On communique l'orientation de la porte
					data.idf = C_STATE_DOOR_ORIENTATION;
					data.value = sens;
					parent->SendToManager(MSG_CHANGE_STATE_VAR, (void*)&data);

					parent->GetEntityManager()->targetDoor = target_door;
					parent->GetEntityManager()->targetMap = target_map;

					// Lock de l'user input
					data.idf = C_STATE_LOCK_USER_INPUT;
					data.value = 1;
					parent->SendToManager(MSG_CHANGE_STATE_VAR, (void*)&data);
					break;
				};

				break;
			}
		}
	}
}


