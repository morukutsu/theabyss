// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "AssetsPreprocessing.h"
#include "mkengine.h"
#include "RessourceManager.h"
#include <vector>
#include <iostream>

namespace mk
{
	// Génération des .txt des tilesets, permettant de déterminer quels sont les tiles transparents
	void AssetsPreprocessing::GenerateTilesetMetafile(std::string tileset_filename, std::string target_filename)
	{
		// Si on est en mode TOC, alors les métadonnées sont déjà générées ...
		if(mk::RessourceManager::getInstance()->mLoadingMode)
			return;

		// On cherche si le fichier meta n'existe pas déjà
		FILE* ft = fopen(target_filename.c_str(), "r");
		if(ft)
		{
			fclose(ft);
			return;
		}

		std::cout << "[ASSETS] Generation des metadonnees du tileset " << tileset_filename << " dans le fichier " << target_filename << std::endl;

		// Chargement du fichier dans une image 
		sf::Image img;
		img.loadFromFile(tileset_filename);

		// Ouverture d'un fichier
		FILE* f = fopen(target_filename.c_str(), "w+");
		if(!f)
			return;

		// Processing
		int tilesize = 64; // pixels

		// Pour chaque tile du tileset
		for(int ty = 0; ty < img.getSize().y/tilesize; ty++)
		{
			for(int tx = 0; tx < img.getSize().x/tilesize; tx++)
			{
				// On cherche si le pixel est opaque ou pas
				bool found = false;
				for(int x = 0; x < tilesize; x++)
				{
					for(int y = 0; y < tilesize; y++)
					{
						sf::Color pixel = img.getPixel(tx*tilesize + x, ty*tilesize + y);
						if(pixel.a != 255)
						{
							found = true;
							break;
						}
					}

					if(found)
						break;
				}

				// On a trouvé si le tile contient au moins un pixel transparent ou pas
				if(found)
				{
					fprintf(f, "1"); // Transparent
				}
				else
				{
					fprintf(f, "0"); // Opaque
				}
			}
		}

		fclose(f);
	}

	void AssetsPreprocessing::GenerateLowQualityTextures(std::string tex_filename)
	{
		// Si on est en mode TOC, alors les métadonnées sont déjà générées ...
		if(mk::RessourceManager::getInstance()->mLoadingMode)
			return;

		// Génération du nom de fichier pour la low texture
		std::string target_filename_low = tex_filename.substr(0, tex_filename.size() - 4);
		target_filename_low = target_filename_low + "_low.png";

		std::string target_filename_verylow = tex_filename.substr(0, tex_filename.size() - 4);
		target_filename_verylow = target_filename_verylow + "_verylow.png";

		// On cherche si le fichier meta n'existe pas déjà
		FILE* ft = fopen(target_filename_low.c_str(), "r");
		FILE* ft2 = fopen(target_filename_verylow.c_str(), "r");

		if(ft && ft2)
		{
			if(ft)
				fclose(ft);
			if(ft2)
				fclose(ft2);
			return;
		}

		std::cout << "[ASSETS] Generation des textures basse qualite " << tex_filename << " > " << target_filename_low << std::endl;
	
		// Chargement du fichier dans une image 
		sf::Texture img;
		img.loadFromFile(tex_filename);
		img.setSmooth(true);
		sf::Sprite spriteTmp(img);

		// Génération de la texture LOW
		spriteTmp.setScale(0.5f, 0.5f);
		sf::RenderTexture renderTexture;
		renderTexture.create(img.getSize().x/2.0f, img.getSize().y/2.0f);
		renderTexture.clear(sf::Color(0, 0, 0, 0) );
		renderTexture.draw(spriteTmp);
		renderTexture.display();
		sf::Image newImg = renderTexture.getTexture().copyToImage();
		newImg.saveToFile(target_filename_low);

		// Génération de la texture VERY LOW
		std::cout << "[ASSETS] Generation des textures basse qualite " << tex_filename << " > " << target_filename_verylow << std::endl;
		spriteTmp.setScale(0.25f, 0.25f);
		sf::RenderTexture renderTexture2;
		renderTexture2.create(img.getSize().x/4.0f, img.getSize().y/4.0f);
		renderTexture2.clear(sf::Color(0, 0, 0, 0) );
		renderTexture2.draw(spriteTmp);
		renderTexture2.display();
		newImg = renderTexture2.getTexture().copyToImage();
		newImg.saveToFile(target_filename_verylow);
	
	}
};