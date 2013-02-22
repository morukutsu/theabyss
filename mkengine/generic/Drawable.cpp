// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Drawable.h"

#include "mkengine.h"

namespace mk
{
	void Drawable::DrawBoundingBox()
	{
		BoundingBox box = GetTransformedBoundingBox();

		glPushMatrix();
		
		glTranslatef(posX, posY, posZ);

		glDisable (GL_TEXTURE_2D);
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);

		glColor3f (1.0f, 0.0f, 0.0f);

		glBegin (GL_QUADS);

		float point = 0.1;

		glVertex3f(-point, -point, 0);
		glVertex3f(+point, -point, 0);
		glVertex3f(+point, +point, 0);
		glVertex3f(-point, +point, 0);

		glEnd();

		glColor3f (0.0f, 1.0f, 0.0f);

		glBegin (GL_LINE_STRIP);

		glVertex3f(box.x1, box.y1, 0);
		glVertex3f(box.x2, box.y1, 0);
		glVertex3f(box.x2, box.y2, 0);
		glVertex3f(box.x1, box.y2, 0);
		glVertex3f(box.x1, box.y1, 0);

		glEnd();

		glDisable (GL_BLEND);
		glEnable (GL_DEPTH_TEST);

		glPopMatrix();
	}

	BoundingBox Drawable::GetTransformedBoundingBox()
	{
		BoundingBox box = bounds;

		// Scale
		box.x1 *= scaleX;
		box.x2 *= scaleX;
		box.y1 *= scaleY;
		box.y2 *= scaleY;

		return box;
	}
};