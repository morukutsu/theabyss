// project_abyss
// LittleSpace Studio 2012

#include "SpineModel.h"
#include <spine/extension.h>

using sf::Quads;
using sf::RenderTarget;
using sf::RenderStates;

void _AtlasPage_createTexture (AtlasPage* self, const char* path) {
	mk::Image* texture = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(path);
	self->rendererObject = texture;
	self->width = texture->getImageWidth();
	self->height = texture->getImageHeight();
}

void _AtlasPage_disposeTexture (AtlasPage* self) {
	//delete (mk::Image*)self->texture;
}

char* _Util_readFile (const char* path, int* length) {
	mk::AsciiFile* atlasFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(path);
	//mk::RessourceManager::getInstance()->DeleteRessource(atlasFile);
	*length = atlasFile->getSize();
	char* ptr = (char*)malloc(*length);

	// TODO : trouver où ce buffer est libéré...
	strncpy(ptr, atlasFile->buffer, atlasFile->getSize());

	return ptr;
}

namespace mk {

SpineModel::SpineModel () 
{
	currentAnim = NULL;
	Bone_setYDown(true);
	anim_finished = false;
}

SpineModel::~SpineModel()
{
	//delete atlas_file;
	AnimationState_dispose(state);
	Skeleton_dispose(skeleton_file);
	SkeletonData_dispose(skeletonData);
}

void SpineModel::Draw ()
{
	skeleton_file->flipX = mirrorX;
	skeleton_file->flipY = mirrorY;

	vertexArray.clear();
	float vertexPositions[8];
	for (int i = 0; i < skeleton_file->slotCount; ++i) {
		Slot* slot = skeleton_file->slots[i];
		Attachment* attachment = slot->attachment;
		if (!attachment || attachment->type != ATTACHMENT_REGION) continue;
		RegionAttachment* regionAttachment = (RegionAttachment*)attachment;
		RegionAttachment_computeVertices(regionAttachment, slot, vertexPositions);

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

		vertices[0].position.x = vertexPositions[VERTEX_X1];
		vertices[0].position.y = vertexPositions[VERTEX_Y1];
		vertices[1].position.x = vertexPositions[VERTEX_X2];
		vertices[1].position.y = vertexPositions[VERTEX_Y2];
		vertices[2].position.x = vertexPositions[VERTEX_X3];
		vertices[2].position.y = vertexPositions[VERTEX_Y3];
		vertices[3].position.x = vertexPositions[VERTEX_X4];
		vertices[3].position.y = vertexPositions[VERTEX_Y4];

		sf::Vector2u size;
		texture = (mk::Image*)((AtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
		size.x = texture->getImageWidth();
		size.y = texture->getImageHeight();
		vertices[0].texCoords.x = regionAttachment->uvs[VERTEX_X1]/* * size.x*/;
		vertices[0].texCoords.y = regionAttachment->uvs[VERTEX_Y1]/* * size.y*/;
		vertices[1].texCoords.x = regionAttachment->uvs[VERTEX_X2]/* * size.x*/;
		vertices[1].texCoords.y = regionAttachment->uvs[VERTEX_Y2]/* * size.y*/;
		vertices[2].texCoords.x = regionAttachment->uvs[VERTEX_X3]/* * size.x*/;
		vertices[2].texCoords.y = regionAttachment->uvs[VERTEX_Y3]/* * size.y*/;
		vertices[3].texCoords.x = regionAttachment->uvs[VERTEX_X4]/* * size.x*/;
		vertices[3].texCoords.y = regionAttachment->uvs[VERTEX_Y4]/* * size.y*/;

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
	
	if(anim_mode == ANIM_ONCE && state->time > currentAnim->duration)
	{
		animationTime = currentAnim->duration;
		state->time = animationTime;
		anim_finished = true;
	}

	if(!anim_finished)
	{
		AnimationState_apply(state, skeleton_file);
		Skeleton_updateWorldTransform(skeleton_file);
	}
}

void SpineModel::PlayAnim(std::string anim, int mode)
{
	currentAnim = SkeletonData_findAnimation(skeletonData, anim.c_str() );
	AnimationState_setAnimation(state, currentAnim, true);
	animationTime = 0;
	anim_mode = mode;

	anim_finished = false;
}

void SpineModel::Play()
{
	oldAnimationTime = animationTime;
	animationTime += 1.0f/30.0f;

	//Skeleton_update(skeleton_file, (1.0f/30.0f));

	//AnimationState_update(state, (1.0f/30.0f));
	//state->time = animationTime;

	if(anim_mode == ANIM_ONCE && animationTime > currentAnim->duration)
	{
		animationTime = currentAnim->duration;
		anim_finished = true;
	}
}

void SpineModel::SavePositions()
{

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
	atlas_file = Atlas_readAtlasFile(atlas.c_str() );

	SkeletonJson* json = SkeletonJson_create(atlas_file);

	skeletonData = SkeletonJson_readSkeletonDataFile(json, skeleton.c_str() );
	SkeletonJson_dispose(json);

	skeleton_file = Skeleton_create(skeletonData);
	state = AnimationState_create(0);

	// Chargement du squelette
	skeleton_file->flipX = false;
	skeleton_file->flipY = false;
	Skeleton_setToSetupPose(skeleton_file);

	skeleton_file->root->x = 0;
	skeleton_file->root->y = 0;
	Skeleton_updateWorldTransform(skeleton_file);

	// Initialisation
	vertexArray.setPrimitiveType(Quads);
	vertexArray.resize(skeleton_file->boneCount * 4);
}

}