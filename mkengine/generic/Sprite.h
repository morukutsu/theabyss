// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_SPRITE
#define MK_SPRITE

#include "lowDisplay.h"
#include "Drawable.h"

namespace mk
{
    struct sSpriteAnim
    {
        //L'unité des positon est un multiple de la taille des frames
        int posy;           //Ligne de l'animation
        int startx;         //Colonne
        int lengh;          //Taille de l'animation
        float fps;            //Vitesse de l'animation en frames
    };
	
	class Image;
    class Sprite : public Drawable
    {
		public:
			Sprite();
			~Sprite();

			void Draw();
			void Draw3D();
			void Play(float dt);
			void Clip(float x, float y, float w, float h);
			void Tint(unsigned char, unsigned char, unsigned char);
			void Tint(u32 color);
			void Alpha(float val);
			void Mirror(bool v, bool h);
			void SetRotCenter(float x, float y);
			void SetCenter(float x, float y);
			void Translate(float x, float y);
			void MoveTo(float x, float y);
			void SetSize(float w, float h);
			void Scale(float w, float h);
			void Assign(Image* img);
			void Rotate(float Angle);
			void SetDepth(float z);

			int  AddAnimation(int py, int stx, int len, float fps);
			void PlayAnim(int mode, int anim);
			void SetAnimationSpeed(int animation, float speed);
			void SetFrameWH(int w, int h);
			void Pause();
			void Restart();
			void Stop();

			void Set3DMode(bool mode) { is3DMode = mode; };
			void SetDepthTest(bool depthTest) { isDepthTest = depthTest; };
			void SavePositions();
			void Interpolate(float dt);

		public:
			sSpriteAnim animations[16];              //Tableau d'animations du sprite
			int frameW, frameH;
			int animNbr, currentAnim, defaultAnim;   //Nombre d'animations dans le sprite
			int currentFrame;                        //Compteur de frames et frame courante
			float frameCounter;
			bool isStarted, isPaused, isLoop;        //Booléeins de depart et de pause pour les animations

			bool is3DMode;

			float dispCenterX, dispCenterY;

			Image* image;                 //Pointeur vers l'image
			int isTexture;	

			bool isDepthTest;
    };

}

#endif
