// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "DrawableMesh.h"

namespace mk
{
	DrawableMesh::DrawableMesh() : Drawable()
	{
		vertexMode = MK_TRIANGLES;
		priority = 0;

		// Bounding box
		bounds.x1 = bounds.y1 = FLT_MAX;
		bounds.x2 = bounds.y2 = FLT_MIN;
	}

	DrawableMesh::~DrawableMesh()
	{
		verts.swap(verts);
		verts.shrink_to_fit();
	}

	void DrawableMesh::AddVertex(mk::Vertex v) 
	{ 
		// Calculate bbox
		if(v.point.x < bounds.x1)
			bounds.x1 = v.point.x;
		if(v.point.y < bounds.y1)
			bounds.y1 = v.point.y;
		if(v.point.x > bounds.x2)
			bounds.x2 = v.point.x;
		if(v.point.y > bounds.y2)
			bounds.y2 = v.point.y;

		verts.push_back(v); 
	}

	void DrawableMesh::Draw()
	{
		lowDisable(MK_TEXTURE_2D);
		lowEnable(MK_BLEND);
		lowEnable(MK_DEPTH_TEST);

		lowPushMatrix();
		lowTranslate3f(posX, posY, posZ + priority*PRIORITY_FACTOR);

		lowBegin(vertexMode);
			for(int k = 0; k < verts.size(); k++)
			{
				Color& c = verts[k].color;
				Point& p = verts[k].point;

				lowSetCurrentColor(c.r, c.g, c.b, c.a);
				lowVertex3f(p.x, p.y, p.z);
			}
		lowEnd();
		lowPopMatrix();

		lowDisable(MK_BLEND);
		lowDisable(MK_DEPTH_TEST);
	}

	void DrawableMesh::MoveTo(float x, float y, float z)
	{
		posX = x;
		posY = y;
		posZ = z;
	}

	void DrawableMesh::Interpolate(float dt)
	{

	}
};