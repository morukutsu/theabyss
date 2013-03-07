// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_DRAWABLE
#define MK_DRAWABLE

#define ANIM_LOOP        0
#define ANIM_ONCE        1

#define PRIORITY_FACTOR  0.01F

namespace mk
{
	enum { DRAWABLE_TYPE_SIMPLE, DRAWABLE_TYPE_LIGHT, DRAWABLE_TYPE_MAP };

	struct State
	{
		float posX, posY, posZ;       // Position X dans le repère
		float scaleX, scaleY;         // Facteur de redimensionnement de l'image
		float angle;                  // Angle de rotation
		float alpha;				  // Alpha
	};

	struct BoundingBox
	{
		float x1, y1, x2, y2;
	};

	static inline bool AABBCollide(float px1, float py1, BoundingBox* a, float px2, float py2, BoundingBox* b)
	{
		if(px1 + a->x2 < px2 + b->x1 || 
					py1 + a->y2 < py2 + b->y1 || 
					px1 + a->x1 > px2 + b->x2 || 
					py1 + a->y1 > py2 + b->y2) 
		{
			return false;
		}
		return true;
	}

	class Shader;
	class Drawable
	{
		public:
			Drawable()
			{
				blending = 0;
				mType = DRAWABLE_TYPE_SIMPLE;
				priority = 0;
				posX = posY = posZ = 0.0f;
				scaleX = scaleY = 1.0f;
				ignoreLightPipeline = false;
				mDepth = 0;
				angle = 0.0f;
				mirrorX = mirrorY = false;
				centerX = centerY = 0;

				isCulled = isCullingIgnored = false;

				isOpaque = false;

				isStencilClipped = false;

				currentShader = 0;
				isShader = false;

				InitState(prevPos);
				InitState(curPos);
			}

			virtual ~Drawable() {};
			virtual void Draw() = 0;
			virtual void Interpolate(float dt) = 0;
			
			//void SetDepth(int depth) { mDepth = depth; }
			void Show() { isVisible = true; };
			void Hide() { isVisible = false; };
			void SetPriority(int p) { priority = p; };
			void SetBlending(int mode) { blending = mode; };
			void SetType(int type) { mType = type; };
			void SetStencilClipped(bool v) { isStencilClipped = v; };
			void SetShader(Shader* shader, bool toggle) { currentShader = shader; isShader = toggle; };
			BoundingBox GetTransformedBoundingBox();

			void InitState(State& s)
			{
				s.posX = posX;
				s.posY = posY;
				s.posZ = posZ;
				s.alpha = alpha;
				s.angle = angle;
				s.scaleX = scaleX;
				s.scaleY = scaleY;
			}

			void DrawBoundingBox();

		public:
			int mType;

			int mDepth; //Profondeur de l'objet, 0 = plus près
			float centerX, centerY;         //Centre de rotation et de scaling

			float posX, posY, posZ;       //Position X dans le repère
			float scaleX, scaleY;         //Facteur de redimensionnement de l'image
			float angle;                  //Angle de rotation

			State prevPos, curPos;

			BoundingBox bounds, transformedBounds;

			bool mirrorX, mirrorY;        //Mirroir du sprite
			int mirrorXVal, mirrorYVal;   //Valeur du mirror
			float alpha;
			float cx1, cx2, cy1, cy2;
			unsigned char r, g, b;
			bool isVisible, isCulled, isCullingIgnored, isStencilClipped;
			int priority;				  // Draw priority
			bool ignoreLightPipeline;
			int blending;
			bool isOpaque;
			Shader* currentShader;
			bool isShader;
	};
};

#endif