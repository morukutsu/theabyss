// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "lowDisplay.h"
#include "lowSystem.h"
#include "MeshBone.h"
#include "MeshBoneAnim.h"
#include "ExtendedSpriteRessource.h"

float		mAutoDepth;
float		mCurrentColor_R, mCurrentColor_G, mCurrentColor_B, mCurrentColor_A;

// Depth
void lowIncrementAutoDepth()
{
	mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayFillRect(float x1, float y1, float x2, float y2, u32 color)
{
	glDisable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	
	//To do : a changer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin (GL_QUADS);
		glColor4ub((color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
		glVertex3f(x1, y1, mAutoDepth);
		glVertex3f(x1, y2, mAutoDepth);
		glVertex3f(x2, y2, mAutoDepth);
		glVertex3f(x2, y1, mAutoDepth);
	glEnd ();
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	//mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayGradientRect(float x1, float y1, float x2, float y2, u32 c1, u32 c2, u32 c3, u32 c4)
{
	glDisable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	
	//To do : a changer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin (GL_QUADS);
		glColor4ub((c1>>24)&0xFF, (c1>>16)&0xFF, (c1>>8)&0xFF, c1&0xFF);
		glVertex3f(x1, y1, mAutoDepth);
		glColor4ub((c2>>24)&0xFF, (c2>>16)&0xFF, (c2>>8)&0xFF, c2&0xFF);
		glVertex3f(x1, y2, mAutoDepth);
		glColor4ub((c3>>24)&0xFF, (c3>>16)&0xFF, (c3>>8)&0xFF, c3&0xFF);
		glVertex3f(x2, y2, mAutoDepth);
		glColor4ub((c4>>24)&0xFF, (c4>>16)&0xFF, (c4>>8)&0xFF, c4&0xFF);
		glVertex3f(x2, y1, mAutoDepth);
	glEnd ();
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
}

void lowDisplayLine(float x1, float y1, float x2, float y2, u32 color)
{
	glDisable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glPushMatrix( );
		glLineWidth(1);
		glTranslatef(0, 0, mAutoDepth);
		glColor4ub((color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
    glPopMatrix( );
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	//mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayImg(mk::Image* img, float x, float y, float cx, float cy, float cw, float ch, float scale_x, 
				   float scale_y, float angle, u32 color, float rX, float rY, float hX, float hY, bool mx, bool my)
{
	if(!img->mLoaded)
		return;

    // Get the sprite size
	float Width  = static_cast<float>(img->getImageWidth());
	float Height = static_cast<float>(img->getImageHeight());
	float cx1 = cx/Width;
	float cy1 = cy/Height;
	float cx2 = (cx+cw)/Width;
	float cy2 = (cy+ch)/Height;

	float mx1 = mx ? (float)cx2 : (float)cx1;
	float mx2 = mx ? (float)cx1 : (float)cx2;
	float my1 = my ? (float)cy2 : (float)cy1;
	float my2 = my ? (float)cy1 : (float)cy2;

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPushMatrix();

	// Use the "offset trick" to get pixel-perfect rendering
	// see http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
	//glTranslatef(0.375f, 0.375f, 0.f);
	
	
	glTranslatef((Width*scale_x)/2.0f+rX, (Height*scale_y)/2.0f+rY, 0.f);
	glTranslatef(x+hX, y+hY, mAutoDepth);
	glRotatef(angle, 0, 0, 1.0f);

	/*if(mx)
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);*/

	glTranslatef(-(Width*scale_x)/2.0f-rX, -(Height*scale_y)/2.0f-rY, 0.f);
	glScalef(scale_x, scale_y, 1.0f);
	
	img->Bind();
 
    // Draw the sprite's triangles
    glBegin(GL_QUADS);
		 glColor4ub((color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);

         glTexCoord2f(mx1,  my1);    
		 glVertex2f(0, 0);

         glTexCoord2f(mx1, my2);
		 glVertex2f(0, Height);

         glTexCoord2f(mx2, my2);
		 glVertex2f(Width, Height);

         glTexCoord2f(mx2, my1);    
		 glVertex2f(Width, 0) ;
    glEnd();

	glPopMatrix( );
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glDisable (GL_TEXTURE_2D);

	//mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayImg3D(mk::Image* img, float x, float y, float z, float cx, float cy, float cw, float ch, float scale_x, 
				   float scale_y, float angle, u32 color, float rX, float rY, float hX, float hY, bool mx, bool my, bool depthTest)
{
	if(!img->mLoaded)
		return;

    // Get the sprite size
	float Width  = static_cast<float>(img->getImageWidth());
	float Height = static_cast<float>(img->getImageHeight());
	float cx1 = cx/Width;
	float cy1 = cy/Height;
	float cx2 = (cx+cw)/Width;
	float cy2 = (cy+ch)/Height;

	/*float mx1 = mx ? (float)cx2 : (float)cx1;
	float mx2 = mx ? (float)cx1 : (float)cx2;
	float my1 = my ? (float)cy2 : (float)cy1;
	float my2 = my ? (float)cy1 : (float)cy2;*/

	float mx1 = cx1;
	float mx2 = cx2;
	float my1 = cy1;
	float my2 = cy2;

	if(depthTest)
		glEnable (GL_DEPTH_TEST);
	else
		glDisable (GL_DEPTH_TEST);

	glEnable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);

	glPushMatrix();

	// Use the "offset trick" to get pixel-perfect rendering
	// see http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
	//glTranslatef(0.375f, 0.375f, 0.f);
	
	glTranslatef(rX, rY, 0.0f);

	glTranslatef(x, y, z);

	glRotatef(angle, 0, 0, 1.0f);

	glTranslatef(-rX, -rY, 0.f);

	glScalef(scale_x, scale_y, 1.0f);

	if(mx)
		glScalef(-1, 1, 1);
	if(my)
		glScalef(1, -1, 1);
	
	img->Bind();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw the sprite's triangles
    glBegin(GL_QUADS);
		 glColor4ub((color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
		 
         glTexCoord2f(mx1,  my1);    
		 glVertex2f(-Width/2, -Height/2);

         glTexCoord2f(mx1, my2);
		 glVertex2f(-Width/2, Height/2);

         glTexCoord2f(mx2, my2);
		 glVertex2f(Width/2, Height/2);

         glTexCoord2f(mx2, my1);    
		 glVertex2f(Width/2, -Height/2) ;
    glEnd();

	glPopMatrix( );
	glDisable (GL_BLEND);
	if(depthTest)
		glDisable (GL_DEPTH_TEST);
	else
		glEnable (GL_DEPTH_TEST);
	glDisable (GL_TEXTURE_2D);
}

void lowDisplayQuadTex3DPartial(mk::Image* img, float texWidth, float texHeight, float quadWidth, float quadHeight, float x, float y, float z, float cx, float cy, float cw, float ch, 
									bool mx, bool my) {
	// Get the sprite size
	float cx1 = (cx+0.5f)/texWidth;
	float cy1 = (cy+0.5f)/texHeight;
	float cx2 = (cx+cw-0.5f)/texWidth;
	float cy2 = (cy+ch-0.5f)/texHeight;

	float mx1 = mx ? cx2 : cx1;
	float mx2 = mx ? cx1 : cx2;
	float my1 = my ? cy2 : cy1;
	float my2 = my ? cy1 : cy2;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/*glTexCoord2f(mx1,  my1);    
	glVertex2f(x -quadWidth/2, y -quadHeight/2);

    glTexCoord2f(mx1, my2);
	glVertex2f(x -quadWidth/2, y + quadHeight/2);

    glTexCoord2f(mx2, my2);
	glVertex2f(x + quadWidth/2, y + quadHeight/2);

    glTexCoord2f(mx2, my1);    
	glVertex2f(x + quadWidth/2, y -quadHeight/2) ;*/

	glTexCoord2f(mx1,  my1);    
	glVertex2f(x, y);

    glTexCoord2f(mx1, my2);
	glVertex2f(x, y + quadHeight);

    glTexCoord2f(mx2, my2);
	glVertex2f(x + quadWidth, y + quadHeight);

    glTexCoord2f(mx2, my1);    
	glVertex2f(x + quadWidth, y) ;
}

void lowDisplayQuadTex(mk::Image* img)
{
	if(!img->mLoaded)
		return;

	// Get the sprite size
	float Width  = static_cast<float>(img->getImageWidth());
	float Height = static_cast<float>(img->getImageHeight());

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);

	img->Bind();
 
    // Draw the sprite's triangles
    glBegin(GL_QUADS);
		 glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

         glTexCoord2f(0,  0);    
		 glVertex3f(0, 0, mAutoDepth);

         glTexCoord2f(0, 1.0f);
		 glVertex3f(0, Height, mAutoDepth);

         glTexCoord2f(1.0f, 1.0f);
		 glVertex3f(Width, Height, mAutoDepth);

         glTexCoord2f(1.0f, 0);    
		 glVertex3f(Width, 0, mAutoDepth) ;
    glEnd();

	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glDisable (GL_TEXTURE_2D);

	// On incrémente pas l'autodepth car le mode de comparaison est LESS OR EQUAL
	//mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayCircle(float x, float y, float radius, int precision, u32 color)
{
	glDisable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glPushMatrix( );
		glLineWidth(1);
		glTranslatef(x, y, mAutoDepth);
		glColor4ub((color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
		
		float angle = 0.0f;
		float diff = PI/(float)precision;

        glBegin(GL_LINE_STRIP);
			glVertex2f(cosf(angle)*radius, sinf(angle)*radius);
			angle += diff;
			for(int k = 1; k < precision*2+1; k++)
			{		
				glVertex2f(cosf(angle)*radius, sinf(angle)*radius);
				angle += diff;
			}   
        glEnd();

    glPopMatrix( );
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);

	//mAutoDepth += DEPTH_AUTOINCREMENT;
}

void lowDisplayText(mk::Font* font, float x, float y, const char* text, u32 color, float scale)
{
	int mLength = strlen(text);
	float mXPos = x;
	//font->mTex.Alpha((float)color.a/255);
	for(int i = 0; i < mLength; i++)
	{
		unsigned char c = text[i];
		sCharacter& mGfxChar = font->mChars[c];
		//font->mTex.Clip(mGfxChar.x, mGfxChar.y, mGfxChar.w, mGfxChar.h);
		//font->mTex.SetSize((float)mGfxChar.w*scale, (float)mGfxChar.h*scale);
		//font->mTex.MoveTo(mXPos + (float)mGfxChar.xoff*scale, y + (float)mGfxChar.yoff*scale);
		//font->mTex.Tint(color.r, color.g, color.b);
		//font->mTex.Draw();
		lowDisplayImg(font->mTex, mXPos + (float)mGfxChar.xoff*scale, y + (float)mGfxChar.yoff*scale, (float)mGfxChar.x, (float)mGfxChar.y, 
			(float)mGfxChar.w, (float)mGfxChar.h, (float)mGfxChar.w/(float)font->mTex->getImageWidth()*scale, (float)mGfxChar.h/(float)font->mTex->getImageHeight()*scale, 0.0f, color);
		mXPos += (float)mGfxChar.xAdvance*scale;
	}
}

void lowDisplayModelBone(mk::ModelBone* mb)
{
	glPushMatrix();
	glTranslatef(mb->posX, mb->posY, mb->posZ + mb->priority*PRIORITY_FACTOR);

	if(mb->mirrorX)
		glRotatef(180.0f, 0.0, 1.0, 0.0); //temporaire, gestion du mirroring
	else
		glRotatef(0.0f, 0.0, 1.0, 0.0); //temporaire, gestion du mirroring
	
	if(mb->mirrorY)
		glRotatef(180.0f, 1.0, 0.0, 0.0); //temporaire, gestion du mirroring
	else
		glRotatef(0.0f, 1.0, 0.0, 0.0); //temporaire, gestion du mirroring


	glRotatef(mb->angle, 0.0, 0.0, 1.0);

	glScalef(mb->scaleX, mb->scaleY, 1.0f);

	if(!mb->animated)
		mb->skeleton = mb->mMesh->md5file.baseSkel;

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);

	if(mb->image)
	{
		glEnable (GL_TEXTURE_2D);
		mb->image->Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glDisable (GL_TEXTURE_2D);
	}

	//Affichage avec vertex array
	mb->PrepareMesh (&mb->mMesh->md5file.meshes[0], mb->skeleton, mb->vertexArray);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	/*for(std::list<mk::ModelTri>::iterator i = mb->modelTris.begin(); i != mb->modelTris.end(); i++)
	{
		int k = (*i).i;
		for(int j = 0; j < 3; j++)
		{
			float* vtx = mb->vertexArray[mb->mMesh->md5file.meshes[0].triangles[k].index[j]];
			float* vnm = mb->normalArray[mb->mMesh->md5file.meshes[0].triangles[k].index[j]];
					
			glColor4ub(mb->r, mb->g, mb->b, mb->alpha*255);
					
			glTexCoord2f(vtx[3], vtx[4] );
			glVertex3f(vtx[0], vtx[1], 0);
		}
	}*/
	
	glColor4ub(mb->r, mb->g, mb->b, mb->alpha*255);

	glVertexPointer (3, GL_FLOAT, sizeof(GL_FLOAT)*5, mb->vertexArray);

	char *evilPointer = (char *)mb->vertexArray;
	evilPointer+=sizeof(GL_FLOAT)*3;
	glTexCoordPointer(2, GL_FLOAT,sizeof(GL_FLOAT)*5, evilPointer);

	glDrawElements (GL_TRIANGLES, mb->mMesh->md5file.meshes[0].num_tris * 3, GL_UNSIGNED_INT, mb->mMesh->vertexIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//Affichage avec vertex array
	/*for(int i = 0; i < mb->mMesh->md5file.num_meshes; ++i)
	{
		mb->PrepareMesh (&mb->mMesh->md5file.meshes[i], mb->skeleton);
			
		glBegin(GL_TRIANGLES);
		for(int k = 0; k < mb->mMesh->md5file.meshes[i].num_tris; k++)
		{
			for(int j = 0; j < 3; j++)
			{
				float* vtx = mb->vertexArray[mb->mMesh->md5file.meshes[i].triangles[k].index[j]];
				float* vnm = mb->normalArray[mb->mMesh->md5file.meshes[i].triangles[k].index[j]];
					
				glColor4ub(mb->r, mb->g, mb->b, mb->alpha*255);
					
				glTexCoord2f(vtx[3], 1.0f-vtx[4] );
				//glNormal3f(vnm[0], vnm[1], vnm[2] );
				glVertex3f(vtx[0], vtx[1], -vtx[2] );
			}
		}

		//glVertexPointer (3, GL_FLOAT,sizeof(GL_FLOAT)*5, mMesh->vertexArray);

		//glNormalPointer(GL_FLOAT, 0, mMesh->normalArray);

		//char *evilPointer = (char *)mMesh->vertexArray;
		//evilPointer+=sizeof(GL_FLOAT)*3;
		//glTexCoordPointer(2, GL_FLOAT,sizeof(GL_FLOAT)*5, evilPointer);

		//glDrawElements (GL_TRIANGLES, mMesh->md5file.meshes[i].num_tris * 3, GL_UNSIGNED_INT, mMesh->vertexIndices);
		glEnd();
	}*/

	glPopMatrix();

	if(mb->image)
	{
		glDisable (GL_TEXTURE_2D);
	}
	
	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
}

void lowDisplayVtxArray(sf::VertexArray& vtxArray, mk::Image* img, float x, float y, float z, float sx, float sy, float angle, bool mx, bool my)
{
	glPushMatrix();

	glTranslatef(x, y, z);
	glScalef(sx, sy, 1.0f);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);

	img->Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Vertex
	glVertexPointer (2, GL_FLOAT, sizeof(sf::Vertex), &vtxArray[0]);

	// Texcoord
	char *evilPointer = (char *)&vtxArray[0];
	evilPointer+=sizeof(sf::Vector2f) + sizeof(sf::Color);
	glTexCoordPointer(2, GL_FLOAT, sizeof(sf::Vertex), evilPointer);

	// Color
	evilPointer = (char *)&vtxArray[0];
	evilPointer+=sizeof(sf::Vector2f);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sf::Vertex), evilPointer);
	
	// Affichage
	glDrawArrays(GL_QUADS, 0, vtxArray.getVertexCount());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glDisable (GL_TEXTURE_2D);

	glPopMatrix();
}

void lowDisplayExtendedSprite(mk::ExtendedSprite* spr)
{
	
}

void lowDisplayFBO(mk::FBO* fbo, int id, float offsetX, float offsetY, float scaleX, float scaleY, float alpha)
{
	// Display
	float Width  = static_cast<float>(fbo->texs[id].w);
	float Height = static_cast<float>(fbo->texs[id].h);
	float texw = fbo->texs[id].texw;
	float texh = fbo->texs[id].texh;

	glPushMatrix();
	
	//glTranslatef(0.375f, 0.375f, 0.f);
	glTranslatef(offsetX, offsetY, mAutoDepth);
	//glScalef(scaleX, scaleY, 1.0f);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);

	fbo->Bind(id);
 
    // Draw the sprite's triangles
    glBegin(GL_QUADS);
		 glColor4f(1.0f, 1.0f, 1.0f, alpha);

		 glTexCoord2f(0,  (float)Height/texh);    
		 glVertex2f(0, 0);

         glTexCoord2f(0, 0);
		 glVertex2f(0, Height * scaleY);

         glTexCoord2f((float)Width/texw, 0);
		 glVertex2f(Width * scaleX, Height * scaleY);

         glTexCoord2f((float)Width/texw, (float)Height/texh);    
		 glVertex2f(Width * scaleX, 0);
    glEnd();

	glDisable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glDisable (GL_TEXTURE_2D);

	glPopMatrix();
}

// Color
void lowSetCurrentColor(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	mCurrentColor_R = r;
	mCurrentColor_G = g;
	mCurrentColor_B = b;
	mCurrentColor_A = a;
}

float lowGetCurrentAlpha()
{
	return mCurrentColor_A;
}


// Matrices
void lowPushMatrix()
{
	glPushMatrix();
}

void lowPopMatrix()
{
	glPopMatrix();
}

void lowTranslate(float x, float y)
{
	glTranslatef(x, y, mAutoDepth);
}

void lowTranslate3f(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void lowScale(float x, float y)
{
	glScalef(x, y, 1.0f);
}

void lowRotate(float angle)
{
	glRotatef(angle, 0.0, 0.0, 1.0f);
}

// Geometry
void lowVertex3f(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

// Render
void lowInitRenderToTexture(int w, int h)
{
	//Redimensionnement du viewport
	//glViewport(0, 0, w, h);
}

void lowEndRenderToTexture(mk::Image* img, int x, int y, int w, int h, bool clear)
{

}

void lowSetBlendMode(int mode)
{
	switch (mode) {
        case MK_BLEND_ALPHA:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case MK_BLEND_ADD:
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case MK_BLEND_MULTIPLY:
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case MK_BLEND_INVERT:
			glBlendFunc(GL_ZERO, GL_ONE_MINUS_DST_COLOR);
            break;
		case MK_BLEND_IGNORE_ALPHA:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
    }
}

// Camera
void lowCamLookAt(float camX, float camY, float camZ, float lookX, float lookY, float lookZ,
				float upX, float upY, float upZ)
{
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, upX, upY, upZ);
}

// OGL states wrappers
GLenum _lowTranslate_MKCAP_to_OGL(int capability)
{
	GLenum cap;
	switch(capability) 
	{
	case MK_BLEND:
		cap = GL_BLEND;
		break;
	case MK_DEPTH_TEST:
		cap = GL_DEPTH_TEST;
		break;
	case MK_TEXTURE_2D:
		cap = GL_TEXTURE_2D;
		break;
	}

	return cap;
}

void lowEnable(int capability)
{
	glEnable(_lowTranslate_MKCAP_to_OGL(capability));
}

void lowDisable(int capability)
{
	glDisable(_lowTranslate_MKCAP_to_OGL(capability));
}

// Geometry drawing
void lowBegin(int primitive)
{
	GLenum prim;
	switch(primitive) 
	{
	case MK_QUADS:
		prim = GL_QUADS;
		break;
	case MK_TRIANGLES:
		prim = GL_TRIANGLES;
		break;
	case MK_TRIANGLE_STRIP:
		prim = GL_TRIANGLE_STRIP;
		break;
	case MK_TRIANGLE_FAN:
		prim = GL_TRIANGLE_FAN;
		break;
	}

	glBegin(prim);
}

void lowEnd()
{
	glEnd();
}

// Texture filtering
void lowEnableNearestFilter()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Temp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void lowEnableLinearFilter()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Temp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}