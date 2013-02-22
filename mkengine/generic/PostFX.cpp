// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "PostFX.h"

namespace mk
{
	// Outils de manipulation de textures
	void PostFX::Downsample(Image* src, Image* dst, int targetw, int targeth)
	{
		// Resize au plus proche
		float rx, ry;

		rx = src->getImageWidth()/targetw;
		ry = src->getImageHeight()/targeth;

		for(int x = 0; x < targetw; x++)
		{
			for(int y = 0; y < targeth; y++)
			{
				u32 color = src->getPixel(x*rx, y*ry);
				dst->setPixel(x, y, color);
			}
		}

		dst->Flush();
	}

	void PostFX::SimpleBlur(Image* src, Image* dst, int times)
	{
		mk::Image *sourcePtr, *destPtr;
		
		//Buffer temporaire
		mk::Image tmp, tmp2;
		tmp.Create(src->getImageWidth(), src->getImageHeight() );
		tmp2.Create(src->getImageWidth(), src->getImageHeight() );

		sourcePtr = src;
		destPtr = &tmp;

		tmp.Flush();
		tmp2.Flush();

		for(int k = 0; k < times; k++)
		{
			//Modifications des pointeurs
			if(k == 1)
			{
				sourcePtr = &tmp;
				destPtr = &tmp2;
			}

			//Pour chaque pixel de l'image source
			for(int x = 0; x < src->getImageWidth(); x++)
			{
				for(int y = 0; y < src->getImageHeight(); y++)
				{
					//Si on est sur un bord de l'image on copie directement
					if(x == 0 || y == 0 || x == src->getImageWidth()-1 || y == src->getImageHeight()-1)
					{
						destPtr->setPixel(x, y, sourcePtr->getPixel(x, y) );
					}
					else
					{
						//On fait la moyenne des 8 pixels autour
						int r, g, b, a;
						r = g = b = a = 0;
						for(int i = -1; i < 2; i++)
						{
							for(int j = -1; j < 2; j++)
							{
								u32 color = sourcePtr->getPixel(x+i, y+j);
								r += (color>>24)&0xFF;
								g += (color>>16)&0xFF;
								b += (color>>8)&0xFF;
								a += color&0xFF;
							}
						}
						r = r/9;
						g = g/9;
						b = b/9;
						a = a/9;

						//Placement du pixel dans l'image de destination
						u32 col = (u32)((u32)r << 24 | ((u32)g << 16) | ((u32)b << 8) | (u32)(a) );
						destPtr->setPixel(x, y, col);
					}
				}
			}

			destPtr->Flush();

			//Inversion des pointeurs de destination
			mk::Image* t = sourcePtr;
			sourcePtr = destPtr;
			destPtr = t;
		}

		//Copie finale
		for(int x = 0; x < src->getImageWidth(); x++)
			for(int y = 0; y < src->getImageHeight(); y++)
				dst->setPixel(x, y, sourcePtr->getPixel(x, y) );
		dst->Flush();
	}
}; 
