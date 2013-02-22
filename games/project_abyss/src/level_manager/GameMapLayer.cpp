// project_abyss
// LittleSpace Studio 2012

#include "GameMapLayer.h"
#include "GameMap.h"

GameMapLayer::GameMapLayer(GameMap* _map, int _layerID, bool opaque) : Drawable()
{
	map = _map;
	layerID = _layerID;

	posZ = map->GetLayerDepth(layerID);
	r = g = b = 255;
	priority = 0;
	alpha = 1.0f;

	isCullingIgnored = true;
	
	isOpaque = opaque;

	mType = mk::DRAWABLE_TYPE_MAP;
}

GameMapLayer::~GameMapLayer()
{

}

void GameMapLayer::Draw()
{
	posX = map->GetPosX();
	posY = map->GetPosY();

	lowSetBlendMode(MK_BLEND_ALPHA);
	lowEnable (MK_DEPTH_TEST);
	lowEnable (MK_BLEND);
	lowEnable (MK_TEXTURE_2D);

	lowSetCurrentColor(r/255.0f, g/255.0f, b/255.0f, alpha);

	if(isOpaque)
		map->DrawLayer(layerID, 0);
	else
		map->DrawLayer(layerID, 1);

	lowDisable (MK_BLEND);
	lowDisable (MK_DEPTH_TEST);
	lowDisable (MK_TEXTURE_2D);
}

void GameMapLayer::Interpolate(float dt)
{

}