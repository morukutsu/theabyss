// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_LOWDISPLAY
#define MK_LOWDISPLAY

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <math.h>

#include "gctypes.h"
#include "Image.h"
#include "ModelBone.h"
#include "ExtendedSprite.h"
#include "Font.h"
#include <list>
#include "Matrix.h"
#include "FBO.h"

#define PI (3.141592653589793F)
#define DEPTH_AUTOINCREMENT (0.01F)

enum {MK_BLEND_ALPHA, MK_BLEND_ADD, MK_BLEND_MULTIPLY, MK_BLEND_INVERT, MK_BLEND_IGNORE_ALPHA};
enum {MK_BLEND, MK_DEPTH_TEST, MK_TEXTURE_2D};
enum {MK_QUADS, MK_TRIANGLES, MK_TRIANGLE_STRIP, MK_TRIANGLE_FAN};

extern float		mAutoDepth;
extern float		mCurrentColor_R, mCurrentColor_G, mCurrentColor_B, mCurrentColor_A;

// Depth
void lowIncrementAutoDepth();

// Display
void lowDisplayFillRect(float x1, float y1, float x2, float y2, u32 color);
void lowDisplayGradientRect(float x1, float y1, float x2, float y2, u32 c1, u32 c2, u32 c3, u32 c4);
void lowDisplayLine(float x1, float y1, float x2, float y2, u32 color);
void lowDisplayImg(mk::Image* img, float x, float y, float cx, float cy, float cw, float ch, 
				   float scale_x = 1.0f, float scale_y = 1.0f, float angle = 0.0f, u32 color = 0xFFFFFFFF,
				   float rX = 0.0f, float rY = 0.0f, float hX = 0.0f, float hY = 0.0f, bool mx = false, bool my = false);
void lowDisplayImg3D(mk::Image* img, float x, float y, float z, float cx, float cy, float cw, float ch, 
				   float scale_x = 1.0f, float scale_y = 1.0f, float angle = 0.0f, u32 color = 0xFFFFFFFF,
				   float rX = 0.0f, float rY = 0.0f, float hX = 0.0f, float hY = 0.0f, bool mx = false, bool my = false, bool depthTest = true);
void lowDisplayQuadTex3DPartial(mk::Image* img, float texWidth, float texHeight, float quadWidth, float quadHeight, 
								float x, float y, float z, float cx, float cy, float cw, float ch, bool mx, bool my);
void lowDisplayQuadTex(mk::Image* img);
void lowDisplayCircle(float x, float y, float radius, int precision, u32 color);
void lowDisplayText(mk::Font* fnt, float x, float y, const char* text, u32 color, float scale = 1.0f);
void lowDisplayModelBone(mk::ModelBone* mb);
void lowDisplayExtendedSprite(mk::ExtendedSprite* spr);
void lowDisplayFBO(mk::FBO* fbo, int id, float offsetX = 0.0f, float offsetY = 0.0f);
void lowDisplayVtxArray(sf::VertexArray& vtxArray, mk::Image* img, float x, float y, float z, float sx, float sy, float angle, bool mx, bool my);

// Color
void lowSetCurrentColor(float r, float g, float b, float a);
float lowGetCurrentAlpha();

// Matrices
void lowPushMatrix();
void lowPopMatrix();
void lowTranslate(float x, float y);
void lowTranslate3f(float x, float y, float z);
void lowScale(float x, float y);
void lowRotate(float angle);

// Geometry
void lowVertex3f(float x, float y, float z);

// Render
void lowInitRenderToTexture(int w, int h);
void lowEndRenderToTexture(mk::Image* img, int x, int y, int w, int h, bool clear);
void lowSetBlendMode(int mode);

// Camera
void lowCamLookAt(float camX, float camY, float camZ, float lookX, float lookY, float lookZ,
				float upX, float upY, float upZ);

// OGL states wrappers
void lowEnable(int capability);
void lowDisable(int capability);

// Geometry drawing
void lowBegin(int primitive);
void lowEnd();

// Texture filtering
void lowEnableNearestFilter();
void lowEnableLinearFilter();

#endif