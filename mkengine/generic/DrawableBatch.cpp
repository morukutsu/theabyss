// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "DrawableBatch.h"

namespace mk
{
	DrawableBatch::DrawableBatch()
	{
		
	}

	void DrawableBatch::Begin()
	{
		mDrawElements.clear();
	}

	void DrawableBatch::End()
	{

	}

	void DrawableBatch::Add(Drawable* elem)
	{
		DrawTask task(elem, elem->posX, elem->posY, elem->cx1, elem->cy1, elem->cx2, elem->cy2, elem->scaleX,
			elem->scaleY, elem->angle, elem->alpha, elem->centerX, elem->centerY, 0.0f, 0.0f,
			elem->r, elem->g, elem->b, elem->mDepth);

		mDrawElements.push_back(task);
	}

	void DrawableBatch::Draw()
	{
		mDrawElements.sort();

		for(std::list<DrawTask>::iterator i = mDrawElements.begin(); i != mDrawElements.end(); i++)
		{
			DrawTask save((*i).elem, (*i).elem->posX, (*i).elem->posY, (*i).elem->cx1, (*i).elem->cy1, (*i).elem->cx2, (*i).elem->cy2, (*i).elem->scaleX,
			(*i).elem->scaleY, (*i).elem->angle, (*i).elem->alpha, (*i).elem->centerX, (*i).elem->centerY, 0.0f, 0.0f,
			(*i).elem->r, (*i).elem->g, (*i).elem->b, (*i).elem->mDepth);

			//Re-parametrage
			(*i).elem->posX = (*i).x;
			(*i).elem->posY = (*i).y;
			(*i).elem->cx1 = (*i).cx1;
			(*i).elem->cy1 = (*i).cy1;
			(*i).elem->cx2 = (*i).cx2;
			(*i).elem->cy2 = (*i).cy2;
			(*i).elem->scaleX = (*i).scaleX;
			(*i).elem->scaleY = (*i).scaleY;
			(*i).elem->angle = (*i).angle;
			(*i).elem->alpha = (*i).alpha;
			(*i).elem->centerX = (*i).centerX;
			(*i).elem->centerY = (*i).centerY;
			(*i).elem->r = (*i).r;
			(*i).elem->g = (*i).g;
			(*i).elem->b = (*i).b;

			//Affichage
			(*i).elem->Draw();

			//Restauration
			(*i).elem->posX = save.x;
			(*i).elem->posY = save.y;
			(*i).elem->cx1 = save.cx1;
			(*i).elem->cy1 = save.cy1;
			(*i).elem->cx2 = save.cx2;
			(*i).elem->cy2 = save.cy2;
			(*i).elem->scaleX = save.scaleX;
			(*i).elem->scaleY = save.scaleY;
			(*i).elem->angle = save.angle;
			(*i).elem->alpha = save.alpha;
			(*i).elem->centerX = save.centerX;
			(*i).elem->centerY = save.centerY;
			(*i).elem->r = save.r;
			(*i).elem->g = save.g;
			(*i).elem->b = save.b;
		}

		mDrawElements.clear();
	}
};