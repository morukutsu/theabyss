// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_EXTENDEDSPRITE
#define MK_EXTENDEDSPRITE

#include "Drawable.h"
#include <list>
#include "Matrix.h"

namespace mk 
{
	class ExtendedSpriteRessource;
	class DOMInstance;
	class DOMModel;
	class DOMBitmap;
	class DOMLayer;
	class ExtendedSprite;

	// ------------------------
	class ExtSprNode 
	{
		public:
			virtual void Draw() = 0;
		
		public:
			Matrix mtx;
			float centerX, centerY;

			std::list<ExtSprNode*> childs;
			DOMInstance* inst;
			
			bool isRoot;
	};

	// ------------------------
	class ExtSprModel : public ExtSprNode
	{
		public:
			ExtSprModel(ExtendedSprite* main, DOMModel* model);
			void Draw();
			void Play(float dt);

		public:
			DOMModel* modelRes;

			// Animation
			int currentKeyFrame, nextKeyFrame, frameCounter, animStarter;
			float lastTime, curFrameTime;
	};

	// ------------------------
	class ExtSprBitmap : public ExtSprNode
	{
		public:
			ExtSprBitmap(ExtendedSprite* main, DOMBitmap* bitmap);
			void Draw();

			DOMBitmap* bitmapRes;
	};

	// -----------------------
	class ExtSprLayer
	{
		public:
			ExtSprLayer(ExtendedSprite* main, DOMLayer* layer);
			
			DOMLayer* layerRes;
	};

	// ------------------------
	class ExtendedSprite  : public Drawable
	{
		public:
			ExtendedSprite();
			~ExtendedSprite();

			void Assign(ExtendedSpriteRessource* res);
			void Draw();
			ExtSprNode* CreateNodeFromName(std::string name);

			void MoveTo(float x, float y);
			void Rotate(float Angle);

			void Interpolate(float dt);

		public:
			ExtendedSpriteRessource * res;
			ExtSprNode* rootNode;
	};
};

#endif