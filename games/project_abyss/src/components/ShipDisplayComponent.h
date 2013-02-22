// project_abyss
// LittleSpace Studio 2012

#ifndef MK_SHIP_DISPLAY_COMPONENT
#define MK_SHIP_DISPLAY_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include "../physics/Vector.h"

#include <map>
#include <string>

#define SHIP_MAIN_PARTS_COUNT			8
#define SHIP_PARTS_COUNT				10
#define GEN_COUNT						4

enum 
{ 
	SHIP_PART_COCKPIT, SHIP_PART_COCKPIT_HURT, 
	SHIP_PART_VIS, SHIP_PART_VIS_HURT,
	SHIP_PART_REACTEUR_FRONT, SHIP_PART_REACTEUR_FRONT_HURT,
	SHIP_PART_REACTEUR_BACK, SHIP_PART_REACTEUR_BACK_HURT,
	SHIP_FX_LUEUR, SHIP_FX_FLASHLIGHT
};

enum {
	REACTOR_LIGHT_0, REACTOR_LIGHT_1,
	FLASHLIGHT_MASK, BACKLIGHT_MASK
};

enum
{
	GEN_REACT_A_FRONT, GEN_REACT_B_FRONT,
	GEN_REACT_A_BACK, GEN_REACT_B_BACK
};

struct PartPosition {
	float x, y, rx, ry;
};

class ParticleGeneratorComponent;
class PlayerMovementComponent;
class LightComponent;
class ShipDisplayComponent : public Component
{
	public:
		ShipDisplayComponent(PlayerMovementComponent*);
		~ShipDisplayComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		mk::Sprite& getSprite(int constant, bool shadow);

		void Mirror(bool v);
		void PositionSprites(bool v);

		void UpdateReactorsAngle();
		void UpdateReactorsPower();

		void ReadPositionsFromXML();

		void UpdateShadowSprites();

	public:
		// Images
		mk::Image* parts[SHIP_PARTS_COUNT];

		// Sprites
		mk::Sprite parts_sprites[SHIP_PARTS_COUNT];
		mk::Sprite parts_shadow[SHIP_PARTS_COUNT];

		// Positions
		NVector parts_centers[SHIP_PARTS_COUNT];
		NVector parts_positions[SHIP_PARTS_COUNT];

		// Composants gérés par ce composant
		ParticleGeneratorComponent* generators[GEN_COUNT];
		LightComponent *flashLight, *backLight, *reactorLight[2];

		// Effets graphiques
		NVector reactorVelOrig[2], reactorVel[2];
		bool isVaisseauHurt;

		// Mirroring
		bool mirrorH;

		// Pointeurs vers les composants utiles
		PlayerMovementComponent* playerMovementComponent;

		// Gestion de l'orientation du vaisseau
		float sourceAngle, reactorsAngle, reactorAngleMovingSpeed, targetAngleInterp;
		float reactorAngle[2];
		bool oldIsPlayerAccelerated;
		bool isAngleInterpolatingToNeutral, isAngleInterpolatingToMoving;

		// Chargement des positions des sprites depuis le fichier
		std::map<std::string, int> partsLookup;
		std::vector<PartPosition> partsPositionsXML;
};

#endif