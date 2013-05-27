// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Font.h"
#include "RessourceManager.h"
#include "lowSystem.h"

namespace mk 
{
	Font::Font()
	{
		mLoaded = false;
	}

	Font::~Font()
	{
		//TODO : Déchargement
	}

	int Font::LoadFromFile(const char* filename)
	{
		//On charge la police depuis un fichier ascii fnt
		FILE* file = fopen(filename, "r");
		if(file)
		{
			char mTemp[256];
			//On lit la première ligne
			fgets(mTemp, 256, file);
			//On lit la seconde ligne
			fgets(mTemp, 256, file);
			sscanf(mTemp, "common lineHeight=%d", &mLineHeight);
			//On lit la 3ème ligne
			fgets(mTemp, 256, file);
			sscanf(mTemp, "page id=0 file=\"%s\"", mName);
			int nSize = strlen(mName);
			mName[nSize-1] = '\0';

			//On charge la texture correspondante (avec le ressource manager)
			//mTex.LoadFromFile(mName);
			mTex = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(mName);
			printf("%s\n", mName);

			//On lit la 4ème ligne
			fgets(mTemp, 256, file);
			sscanf(mTemp, "chars count=%d", &mCharNbr);

			//On boucle sur tout les caractères
			for(int k = 0; k < mCharNbr; k++)
			{
				int id, x, y, width, height, xoffset, yoffset, xadvance;
				fgets(mTemp, 256, file);
				sscanf(mTemp, "char id=%d   x=%d    y=%d    width=%d     height=%d     xoffset=%d     yoffset=%d    xadvance=%d", &id, &x, &y, &width, &height, &xoffset, &yoffset, &xadvance);
				mChars[id].id = id;
				mChars[id].x = x;
				mChars[id].y = y;
				mChars[id].w = width;
				mChars[id].h = height;
				mChars[id].xoff = xoffset;
				mChars[id].yoff = yoffset;
				mChars[id].xAdvance = xadvance;
			}

			//La police est bien chargée
			fclose(file);
			//std::cout << "Notif : La police " << filename << " est chargee" << std::endl;
			mLoaded = true;
			return 0;
		}
		else
		{
			//std::cout << "Erreur - Le fichier de police n'existe pas" << std::endl;
			return 1;
		}
	}

	int Font::LoadFromFile(FILE* fp, long size)
	{
		//On charge la police depuis un fichier ascii fnt
		FILE* file = fp;
		if(file)
		{
			char mTemp[256];
			char mNameTmp[256];

			//On lit la première ligne
			lowFileGets(mTemp, 256, file);

			//On lit la seconde ligne
			lowFileGets(mTemp, 256, file);
			sscanf(mTemp, "common lineHeight=%d", &mLineHeight);

			//On lit la 3ème ligne
			lowFileGets(mTemp, 256, file);

			sscanf(mTemp, "page id=0 file=\"%s\"", mName);
			int nSize = strlen(mName);
			mName[nSize-1] = '\0';
				
			//mTex.LoadFromFile(mName);
			long sp = mk::RessourceManager::getInstance()->GetCurrentPosition();

			//On charge la texture correspondante (avec le ressource manager)
			mTex = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(mName);

			if(mk::RessourceManager::getInstance()->mLoadingMode == true)
				mk::RessourceManager::getInstance()->SetCurrentPosition(sp);

			//On lit la 4ème ligne
			lowFileGets(mTemp, 256, file);
			sscanf(mTemp, "chars count=%d", &mCharNbr);

			//On boucle sur tout les caractères
			for(int k = 0; k < mCharNbr; k++)
			{
				int id, x, y, width, height, xoffset, yoffset, xadvance;
				fgets(mTemp, 256, file);
				sscanf(mTemp, "char id=%d   x=%d    y=%d    width=%d     height=%d     xoffset=%d     yoffset=%d    xadvance=%d", &id, &x, &y, &width, &height, &xoffset, &yoffset, &xadvance);
				mChars[id].id = id;
				mChars[id].x = x;
				mChars[id].y = y;
				mChars[id].w = width;
				mChars[id].h = height;
				mChars[id].xoff = xoffset;
				mChars[id].yoff = yoffset;
				mChars[id].xAdvance = xadvance;
			}
			//std::cout << "Notif : La police " << filename << " est chargee" << std::endl;
			mLoaded = true;
			return 0;
		}
		else
		{
			//std::cout << "Erreur - Le fichier de police n'existe pas" << std::endl;
			return 1;
		}
	}

	int Font::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}
}
