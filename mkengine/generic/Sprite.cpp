// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Image.h"
#include "Sprite.h"
#include <iostream>
#include "../pc/Shader.h"

namespace mk
{
    Sprite::Sprite() : Drawable()
    {
        centerX = centerY = 0;
        posX = posY = posZ = 0;
        scaleX = scaleY = 1.0f;
        angle = 0.0f;
        mirrorX = mirrorY = false;
        alpha = 1.0f;
		dispCenterX = dispCenterY = 0;

		currentFrame = 0;
        frameCounter = 0.0f;
        animNbr = -1;
        frameW = frameH = 0;
        currentAnim = defaultAnim = 0;
		currentAnim = -1;
        isStarted = isPaused = isLoop = false;

		r = g = b = 255;

		mDepth = 0;

		is3DMode = false;

		isDepthTest = true;
    }

    Sprite::~Sprite()
    {
        //image->FreeImage();
    }

    void Sprite::Draw()
    {
		u32 col = (u32)((u32)r << 24 | ((u32)g << 16) | ((u32)b << 8) | (u32)(alpha*255.0f) );

		if(is3DMode)
		{
			lowDisplayImg3D(image, posX, posY, posZ + priority*PRIORITY_FACTOR, cx1, cy1, cx2, cy2, scaleX, scaleY, angle, 
				col, (float)centerX, (float)centerY, dispCenterX, dispCenterY, mirrorX, mirrorY, isDepthTest);
		}
		else
		{
			lowDisplayImg(image, posX, posY, cx1, cy1, cx2, cy2, scaleX, scaleY, angle, 
				col, (float)centerX, (float)centerY, dispCenterX, dispCenterY, mirrorX, mirrorY);
		}
    }

	void Sprite::Draw3D()
    {
		u32 col = (u32)((u32)r << 24 | ((u32)g << 16) | ((u32)b << 8) | (u32)(alpha*255.0f) );

		lowDisplayImg3D(image, posX, posY, posZ, cx1, cy1, cx2, cy2, scaleX, scaleY, angle, 
			col, (float)centerX, (float)centerY, 0.0f, 0.0f, mirrorX, mirrorY);
    }

    void Sprite::Play(float dt)
	{
        if(isPaused == false && isStarted == true)
        {
            //On update le framecounter
            frameCounter += dt;
            //On vérifie l'état du framecounter
            if (frameCounter > animations[currentAnim].fps)
            {
                frameCounter = 0.0f;
                currentFrame++;
            }
            //On vérifie l'état de la frame courante (cas loop)
            if (isLoop && currentFrame > animations[currentAnim].lengh)
                currentFrame = 0;
            //On vérifie si l'animation est finie
            if (!isLoop && currentFrame > animations[currentAnim].lengh)
                this->Stop();

            //On clippe le sprite
            Clip(animations[currentAnim].startx * frameW + currentFrame * frameW,
                 animations[currentAnim].posy* frameH, frameW, frameH);
        }
    }

    void Sprite::Clip(float x, float y, float w, float h)
    {
        //ulSetImageTileSize(image->img, x, y, w, h);
		cx1 = x;
		cx2 = w;
		cy1 = y;
		cy2 = h;
    }

    void Sprite::Tint(unsigned char cr, unsigned char cg, unsigned char cb)
    {
        r = cr;
		g = cg;
		b = cb;
    }

	void Sprite::Tint(u32 color)
    {
        r = (color >> 24) & 0xFF;
		g = (color >> 16) & 0xFF;
		b = (color >> 8) & 0xFF;
    }

    void Sprite::SetFrameWH(int w, int h)
    {
        frameW = w;
        frameH = h;
        SetSize((float)frameW, (float)frameH);
    }

    int Sprite::AddAnimation(int py, int stx, int len, float fps)
    {
        //Déclaration d'une structure anim temporaire
        sSpriteAnim tmpAnim = {py, stx, len, fps};
        //Ajout au vector
        animations[animNbr] = tmpAnim;
        animNbr++;
        return animNbr;
    }

    void Sprite::PlayAnim(int mode, int anim)
    {
        //Reset de l'ancienne animation
        switch (mode)
        {
        case ANIM_LOOP:
            //Reset de l'ancienne animation
            if (currentAnim != anim)
            {	
				currentFrame = 0;
                frameCounter = 0.0f;
                //Démarrage d'une animation
                isStarted = true;
                isLoop = true;
                //Settings
                currentAnim = anim;
            }
            break;

        case ANIM_ONCE:
            if (currentAnim != anim)
            {
				currentFrame = 0;
                frameCounter = 0.0f;
                //Démarrage d'une animation
                isStarted = true;
                isLoop = false;
                //Settings
                currentAnim = anim;
            }
            break;
        }
    }

    void Sprite::SetAnimationSpeed(int animation, float speed)
    {
        animations[animation].fps = speed;
    }

    void Sprite::Pause()
    {
        isPaused = true;
    }

    void Sprite::Restart()
    {
        isPaused = false;
    }

    void Sprite::Stop()
    {
        frameCounter = 0;
        currentFrame--;
        isStarted = false;
    }

	void Sprite::Assign(Image* img)
    {
		image = img;
        /*scaleX = image->width;
        scaleY = image->height;*/
        isTexture = true;
		cx1 = cy1 = 0;
		cx2 = image->getImageWidth();
		cy2 = image->getImageHeight();

		bounds.x1 = -image->getImageWidth()/2;
		bounds.y1 = -image->getImageHeight()/2;

		bounds.x2 = image->getImageWidth()/2;
		bounds.y2 = image->getImageHeight()/2;
    }

    void Sprite::Scale(float w, float h)
    {
		scaleX = w;
        scaleY = h;
    }

	void Sprite::SetSize(float w, float h)
    {
		scaleX = w/image->getImageWidth();
        scaleY = h/image->getImageHeight();
    }

    void Sprite::Rotate(float Angle)
    {
        angle = Angle;
    }

    void Sprite::MoveTo(float x, float y)
    {
        posX = x;
        posY = y;
    }

    void Sprite::Translate(float x, float y)
    {
        posX += x;
        posY += y;
    }
        
	void Sprite::SetRotCenter(float x, float y)
    {
		centerX = x;
        centerY = y;
    }
        
	void Sprite::Mirror(bool v, bool h)
    {
		mirrorX = v;
        mirrorY = h;	
    }

	void Sprite::SetDepth(float z)
	{
		posZ = z;
	}

    void Sprite::Alpha(float val)
    {
		alpha = val;

		if(alpha > 1.0f)
			alpha = 1.0f;
		if(alpha < 0.0f)
			alpha = 0.0f;
    }

	void Sprite::SavePositions()
	{
		prevPos = curPos;
		curPos.posX = posX;
		curPos.posY = posY;
		curPos.posZ = posZ;
		curPos.alpha = alpha;
		curPos.angle = angle;
		curPos.scaleX = scaleX;
		curPos.scaleY = scaleY;
	}
	
	void Sprite::Interpolate(float dt)
	{
		posX = Lerp(dt, 0.0f, 1.0f, prevPos.posX, curPos.posX);
		posY = Lerp(dt, 0.0f, 1.0f, prevPos.posY, curPos.posY);
		posZ = Lerp(dt, 0.0f, 1.0f, prevPos.posZ, curPos.posZ);
		alpha = Lerp(dt, 0.0f, 1.0f, prevPos.alpha, curPos.alpha);
		scaleX = Lerp(dt, 0.0f, 1.0f, prevPos.scaleX, curPos.scaleX);
		scaleY = Lerp(dt, 0.0f, 1.0f, prevPos.scaleY, curPos.scaleY);

		if(prevPos.angle != curPos.angle)
			angle = RadiansToDegrees(Slerp2D(dt, 0.0f, 1.0f, DegreesToRadians(prevPos.angle), DegreesToRadians(curPos.angle)));
	}

	void Sprite::SetCenter(float x, float y)
	{
		dispCenterX = x;
		dispCenterY = y;
	}
}
