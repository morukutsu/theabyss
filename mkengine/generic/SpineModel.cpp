// project_abyss
// LittleSpace Studio 2012

#include "SpineModel.h"
#include <spine/extension.h>

using sf::Quads;
using sf::RenderTarget;
using sf::RenderStates;

namespace spine {
void _AtlasPage_createTexture (spine::AtlasPage* self, const char* path) {
	mk::Image* texture = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(path);
	self->texture = texture;
	self->width = texture->getImageWidth();
	self->height = texture->getImageHeight();
}

void _AtlasPage_disposeTexture (spine::AtlasPage* self) {
	//delete (mk::Image*)self->texture;
}

char* _Util_readFile (const char* path, int* length) {
	mk::AsciiFile* atlasFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(path);
	//mk::RessourceManager::getInstance()->DeleteRessource(atlasFile);
	*length = atlasFile->getSize();
	char* ptr = (char*)malloc(*length);

	// TODO : trouver où ce buffer est libéré...
	strcpy(ptr, atlasFile->buffer);

	return ptr;
}

}

namespace mk {

SpineModel::SpineModel () 
{
	currentAnim = NULL;
	spine::Bone_setYDown(true);
}

SpineModel::~SpineModel()
{
	//delete atlas_file;
	spine::AnimationState_dispose(state);
	spine::Skeleton_dispose(skeleton_file);
	spine::SkeletonData_dispose(skeletonData);
}

void SpineModel::Draw ()
{
	vertexArray.clear();
	for (int i = 0; i < skeleton_file->slotCount; ++i) {
		spine::Slot* slot = skeleton_file->slots[i];
		spine::Attachment* attachment = slot->attachment;
		if (!attachment || attachment->type != spine::ATTACHMENT_REGION) continue;
		spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;
		RegionAttachment_updateVertices(regionAttachment, slot);

		sf::Uint8 r = skeleton_file->r * slot->r * 255;
		sf::Uint8 g = skeleton_file->g * slot->g * 255;
		sf::Uint8 b = skeleton_file->b * slot->b * 255;
		sf::Uint8 a = skeleton_file->a * slot->a * 255;

		sf::Vertex vertices[4];
		vertices[0].color.r = r;
		vertices[0].color.g = g;
		vertices[0].color.b = b;
		vertices[0].color.a = a;
		vertices[1].color.r = r;
		vertices[1].color.g = g;
		vertices[1].color.b = b;
		vertices[1].color.a = a;
		vertices[2].color.r = r;
		vertices[2].color.g = g;
		vertices[2].color.b = b;
		vertices[2].color.a = a;
		vertices[3].color.r = r;
		vertices[3].color.g = g;
		vertices[3].color.b = b;
		vertices[3].color.a = a;

		vertices[0].position.x = regionAttachment->vertices[spine::VERTEX_X1];
		vertices[0].position.y = regionAttachment->vertices[spine::VERTEX_Y1];
		vertices[1].position.x = regionAttachment->vertices[spine::VERTEX_X2];
		vertices[1].position.y = regionAttachment->vertices[spine::VERTEX_Y2];
		vertices[2].position.x = regionAttachment->vertices[spine::VERTEX_X3];
		vertices[2].position.y = regionAttachment->vertices[spine::VERTEX_Y3];
		vertices[3].position.x = regionAttachment->vertices[spine::VERTEX_X4];
		vertices[3].position.y = regionAttachment->vertices[spine::VERTEX_Y4];

		sf::Vector2u size;
		texture = (mk::Image*)regionAttachment->texture;
		size.x = texture->getImageWidth();
		size.y = texture->getImageHeight();
		vertices[0].texCoords.x = regionAttachment->uvs[spine::VERTEX_X1]/* * size.x*/;
		vertices[0].texCoords.y = regionAttachment->uvs[spine::VERTEX_Y1]/* * size.y*/;
		vertices[1].texCoords.x = regionAttachment->uvs[spine::VERTEX_X2]/* * size.x*/;
		vertices[1].texCoords.y = regionAttachment->uvs[spine::VERTEX_Y2]/* * size.y*/;
		vertices[2].texCoords.x = regionAttachment->uvs[spine::VERTEX_X3]/* * size.x*/;
		vertices[2].texCoords.y = regionAttachment->uvs[spine::VERTEX_Y3]/* * size.y*/;
		vertices[3].texCoords.x = regionAttachment->uvs[spine::VERTEX_X4]/* * size.x*/;
		vertices[3].texCoords.y = regionAttachment->uvs[spine::VERTEX_Y4]/* * size.y*/;

		vertexArray.append(vertices[0]);
		vertexArray.append(vertices[1]);
		vertexArray.append(vertices[2]);
		vertexArray.append(vertices[3]);
	}
	lowDisplayVtxArray(vertexArray, texture, posX, posY, posZ, scaleX, scaleY, angle, mirrorX, mirrorY);
}

void SpineModel::Interpolate(float dt)
{
	oldAnimationTime = animationTime;

	state->time = animationTime + (1.0f/30.0f)*dt;
	
	//animationTime += (1.0f/30.0f)*dt;
	
	//spine::AnimationState_update(state, animationTime - oldAnimationTime);

	if(anim_mode == ANIM_LOOP)
	{
		spine::AnimationState_apply(state, skeleton_file);
		spine::Skeleton_updateWorldTransform(skeleton_file);
	}
	else if(anim_mode == ANIM_ONCE && animationTime < currentAnim->duration)
	{
		spine::AnimationState_apply(state, skeleton_file);
		spine::Skeleton_updateWorldTransform(skeleton_file);
	}
}

void SpineModel::PlayAnim(std::string anim, int mode)
{
	currentAnim = spine::SkeletonData_findAnimation(skeletonData, anim.c_str() );
	spine::AnimationState_setAnimation(state, currentAnim, true);
	animationTime = 0;
	anim_mode = mode;
}

void SpineModel::Play()
{
	oldAnimationTime = animationTime;
	animationTime += 1.0f/30.0f;

	//spine::Skeleton_update(skeleton_file, (1.0f/30.0f));

	//spine::AnimationState_update(state, (1.0f/30.0f));
	//state->time = animationTime;

	if(anim_mode == ANIM_ONCE && animationTime > currentAnim->duration)
	{
		animationTime = currentAnim->duration;
	}
}


void SpineModel::LoadModelDescriptorFile(std::string filename)
{
	mk::AsciiFile * f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);

	TiXmlDocument doc;

	doc.Parse(f->getString() );

	TiXmlElement * root = doc.FirstChildElement("SpineModel");
	TiXmlElement * elem;

	// Clearing our data structures

	// Reading from XML file
	std::string atlas, skeleton;

	elem = root->FirstChildElement("Atlas");
	atlas = elem->GetText();

	elem = root->FirstChildElement("Skeleton");
	skeleton = elem->GetText();

	// Chargement de l'Atlas
	atlas_file = spine::Atlas_readAtlasFile(atlas.c_str() );
	spine::SkeletonJson* json = spine::SkeletonJson_create(atlas_file);
	skeletonData = SkeletonJson_readSkeletonDataFile(json, skeleton.c_str() );
	spine::SkeletonJson_dispose(json);

	skeleton_file = Skeleton_create(skeletonData);
	state = spine::AnimationState_create(0);

	// Chargement du squelette
	skeleton_file->flipX = false;
	skeleton_file->flipY = false;
	spine::Skeleton_setToBindPose(skeleton_file);

	skeleton_file->root->x = 0;
	skeleton_file->root->y = 0;
	spine::Skeleton_updateWorldTransform(skeleton_file);

	// Initialisation
	vertexArray.setPrimitiveType(Quads);
	vertexArray.resize(skeleton_file->boneCount * 4);
}

}