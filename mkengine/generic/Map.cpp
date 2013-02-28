// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Map.h"
#include "mkengine.h"
#include <sstream>

namespace mk
{
	Map::Map()
	{
		posX = posY = 0;
		bgColor = 0x000000FF;
		mapScale = 32;
		backgroundImage = new Sprite();
		isBgImage = false;
		isBgColor = false;
		isNoEntities = false;
	}

	Map::~Map()
	{
		delete backgroundImage;

		for(std::vector<mk::Sprite*>::iterator it = tilesets_spr.begin(); it != tilesets_spr.end(); it++)
		{
			delete (*it);
		}
	}

	void Map::SetTextureQuality(int quality)
	{
		textureQuality = quality;

		switch(quality)
		{
		case 0:
			textureScale = 4.0f;
			break;
		case 1:
			textureScale = 2.0f;
			break;
		case 2:
		default:
			textureScale = 1.0f;
			textureQuality = 2;
			break;
		}
	}

	void Map::ParseMap(std::string filename)
	{
		// Chargement du fichier en mémoire
		mk::AsciiFile * f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);
		map.ParseText(f->getString() );
		// Suppression de la map en cache dans le Ressource Manager
		mk::RessourceManager::getInstance()->DeleteRessource(f);

		// Récupération du dossier ou se trouve la map (pour la gestion des liens en relatif)
		racine = "";
		size_t pos = filename.find_last_of('/');
		racine = filename.substr(0, pos+1);

		// Débug de certaines informations sur la map
		std::cout << "[Load] Parsing map : " << filename << "(" << map.GetWidth() << " x " << map.GetHeight() << ") layers : " << map.GetNumLayers() << std::endl;
	}

	void Map::Load()
	{
		// Chargement des sprites correspondant aux tilesets dans une hashmap
		for(int k = 0; k < map.GetNumTilesets(); k++) {
			mk::Sprite* spr = new mk::Sprite();

			std::string tfilename = racine + map.GetTileset(k)->GetImage()->GetSource();

			// Chargement de la bonne texture en fonction de la qualité choisie
			std::string real_texture_filename = tfilename;
			if(textureQuality == 1)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_low.png";
			} 
			else if(textureQuality == 0)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_verylow.png";
			}

			std::string metafilename = racine + map.GetTileset(k)->GetImage()->GetSource();
			metafilename = metafilename.substr(0, metafilename.size() - 3);
			metafilename = metafilename + "txt";

			// Creation des textures low quality
			mk::AssetsPreprocessing::getInstance()->GenerateLowQualityTextures("data/" + tfilename);

			spr->Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(real_texture_filename) );
			tilesets_spr.push_back(spr);

			// Création / Chargement du fichier meta
			mk::AssetsPreprocessing::getInstance()->GenerateTilesetMetafile("data/" + tfilename, "data/" + metafilename);

			LoadTilesetMeta(metafilename, spr);
		}

		// BG Image
		std::string bgImageStr = map.GetProperties().GetLiteralProperty("bgimage");
		if(bgImageStr != "No such property!") {
			// Creation des textures low quality
			mk::AssetsPreprocessing::getInstance()->GenerateLowQualityTextures("data/" + racine + bgImageStr);

			std::string real_texture_filename = racine + bgImageStr;
			if(textureQuality == 1)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_low.png";
			} 
			else if(textureQuality == 0)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_verylow.png";
			}

			backgroundImage->Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(real_texture_filename));
			isBgImage = true;
		}

		// Propriétés générales de la map
		// BG Color
		std::string bgColorStr = map.GetProperties().GetLiteralProperty("bgcolor");
		if(bgColorStr != "No such property!") {
			std::stringstream ss;
			ss << std::hex << bgColorStr;
			ss >> bgColor;
			bgColor = bgColor << 8 | 0xFF;
			isBgColor = true;
		}

		// Lighting
		isLighting = false;
		std::string lightingStr = map.GetProperties().GetLiteralProperty("lighting");
		if(lightingStr == "1") 
		{
			isLighting = true;
		} 
		else
		{
			isLighting = false;
		}

		// Mode no entities
		isNoEntities = false;
		std::string noEntitiesStr = map.GetProperties().GetLiteralProperty("no_entities");
		if(noEntitiesStr == "1") 
			isNoEntities = true;
		else
			isNoEntities = false;

		// Culling par défaut
		SetCullArea(0, 0, map.GetWidth(), map.GetHeight() );

		std::cout << "[Load] Map base loaded.\n";
	}

	std::list<std::string> Map::GetResourcesToLoad() 
	{
		std::list<std::string> resourcesToLoad;

		// Chargement des sprites correspondant aux tilesets dans une hashmap
		for(int k = 0; k < map.GetNumTilesets(); k++) {
			std::string tfilename = racine + map.GetTileset(k)->GetImage()->GetSource();
			std::string real_texture_filename = tfilename;
			if(textureQuality == 1)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_low.png";
			} 
			else if(textureQuality == 0)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_verylow.png";
			}

			resourcesToLoad.push_back(real_texture_filename);
		}

		std::string bgImageStr = map.GetProperties().GetLiteralProperty("bgimage");
		if(bgImageStr != "No such property!") {
			std::string real_texture_filename = racine + bgImageStr;
			if(textureQuality == 1)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_low.png";
			} 
			else if(textureQuality == 0)
			{
				real_texture_filename = real_texture_filename.substr(0, real_texture_filename.size() - 4);
				real_texture_filename = real_texture_filename + "_verylow.png";
			}

			resourcesToLoad.push_back(real_texture_filename);
		}

		return resourcesToLoad;
	}

	void Map::MoveTo(float x, float y)
	{
		posX = x;
		posY = y;
	}

	void Map::Draw()
	{
		// Récupérations des couches de la map
		const std::vector<Tmx::Layer*>& layers = map.GetLayers();

		lowSetBlendMode(MK_BLEND_ALPHA);
		/*glAlphaFunc ( GL_GREATER, 0.5 ) ;
		glEnable ( GL_ALPHA_TEST ) ;*/

		// --
		lowEnable (MK_DEPTH_TEST);
		lowEnable (MK_BLEND);
		lowEnable (MK_TEXTURE_2D);

		// Itération sur les couches de la map
		for(int k = 0; k < layers.size(); k++) 
		{	
			DrawLayer(k, 0);
		}

		// ---
		lowDisable (MK_BLEND);
		lowDisable (MK_DEPTH_TEST);
		lowDisable (MK_TEXTURE_2D);

		/*glDisable ( GL_ALPHA_TEST ) ;*/
	}

	void Map::DrawLayer(int k, int opaque)
	{
		const Tmx::Layer* layer = map.GetLayers()[k];

		// Scale du Layer
		float layerScale = 1.0f;
		std::string scaleStr = layer->GetProperties().GetLiteralProperty("scale");
		if(scaleStr != "No such property!") {
			std::stringstream ss;
			ss << std::hex << scaleStr;
			ss >> layerScale;
		}

		// Itération sur les différents tilesets de la map
		for(int tileset = 0; tileset < map.GetNumTilesets(); tileset++) 
		{
			// Choix de la texture
			mk::Sprite* curTileset = tilesets_spr[tileset];
			mk::TilesetMetadata* curMeta = &tilesets_meta[tileset];

			int tilesetWidthTiles = map.GetTileset(tileset)->GetImage()->GetWidth() / map.GetTileWidth();
			int tilesetHeightTiles = map.GetTileset(tileset)->GetImage()->GetHeight() / map.GetTileHeight();
			int tilesetWidthPixels = map.GetTileset(tileset)->GetImage()->GetWidth();
			int tilesetHeightPixels = map.GetTileset(tileset)->GetImage()->GetHeight();

			bool isTexBound = false;

			// Lecture de la profondeur de la couche
			float layer_depth = (float)layer->GetProperties().GetNumericProperty("depth");

			lowPushMatrix();

			// Depth/scale correction
			lowScale(layerScale, layerScale);
			/*float diffX = ((map.GetWidth() * map.GetTileWidth() * layerScale) - (map.GetWidth() * map.GetTileWidth())) / mapScale / 2.0f;
			float diffY = ((map.GetHeight() * map.GetTileHeight() * layerScale) - (map.GetHeight() * map.GetTileHeight())) / mapScale / 2.0f;*/
			float diffX, diffY;
			diffX = diffY = 0.0f;

			lowTranslate3f(posX - diffX, posY - diffY, layer_depth);
			
			int cullDepthCorrection = 3;
			if(layer_depth > 0.0f)
			{
				// Calcul du nombre de tiles à afficher en plus à l'aide d'une formule totalement empirique
				cullDepthCorrection = (int)layer_depth/1.25f;
			}

			int rcx1 = cx - cullDepthCorrection < 0 ? 0 : cx - cullDepthCorrection;
			int rcy1 = cy - cullDepthCorrection < 0 ? 0 : cy - cullDepthCorrection;
			int rcx2 = cx + cullDepthCorrection + cw > layer->GetWidth() ? layer->GetWidth() : cx + cw + cullDepthCorrection;
			int rcy2 = cy + cullDepthCorrection + ch > layer->GetHeight()  ? layer->GetHeight() : cy + ch + cullDepthCorrection;

			// Itérations sur les tiles de la couche
			for(int x = rcx1; x < rcx2; x++) 
			{
				for(int y = rcy1; y < rcy2; y++) 
				{
					// Récupération du tile et du tileset
					int tilesetIndex = layer->GetTileTilesetIndex(x, y);

					if(tilesetIndex == -1 || tilesetIndex != tileset)
						continue;

					int map_tile_id = layer->GetTileId(x, y);

					int xOnTileset = map_tile_id % tilesetWidthTiles;
					int yOnTileset = map_tile_id / tilesetWidthTiles;
					
					if(curMeta->data[xOnTileset][yOnTileset] != opaque)
						continue;

					if(!isTexBound)
					{
						// Binding de la texture
						curTileset->image->Bind();
						lowEnableLinearFilter();
						isTexBound = true;

						lowBegin(MK_QUADS);
					}

					// Affichage du tile
					lowDisplayQuadTex3DPartial(curTileset->image, 
						tilesetWidthPixels, tilesetHeightPixels,
						map.GetTileWidth() / mapScale, map.GetTileHeight() / mapScale,
						x * map.GetTileWidth() / mapScale, y * map.GetTileHeight() / mapScale, 0, 
						xOnTileset * map.GetTileWidth(), yOnTileset * map.GetTileHeight(), 
						map.GetTileWidth(), map.GetTileHeight(), 
						layer->IsTileFlippedHorizontally(x, y), layer->IsTileFlippedVertically(x, y));
				}
			}
			
			if(isTexBound)
				lowEnd();
			
			lowPopMatrix();
		}
	}

	void Map::DrawBGColor()
	{
		lowPushMatrix();
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), bgColor);
		lowPopMatrix();
	}

	void Map::DrawBGImage()
	{
		if(isBgImage)
		{
			backgroundImage->MoveTo(0, 0);
			backgroundImage->Scale(textureScale, textureScale);
			backgroundImage->Draw();
		}
	}

	float Map::GetLayerDepth(int id)
	{
		const Tmx::Layer* layer = map.GetLayers()[id];
		return (float)layer->GetProperties().GetNumericProperty("depth");
	}

	void Map::SetCullArea(int _cx, int _cy, int _cw, int _ch)
	{
		cx = _cx;
		cy = _cy;
		cw = _cw;
		ch = _ch;
	}

	void Map::LoadTilesetMeta(std::string filename, mk::Sprite* spr)
	{
		mk::AsciiFile* file = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);

		mk::TilesetMetadata meta;
		
		meta.data.resize(spr->image->getImageWidth()*textureScale / 64);
		for(int k = 0; k < meta.data.size(); k++)
			meta.data[k].resize(spr->image->getImageHeight()*textureScale / 64);

		// Chargement des métadonnées 
		int x, y;
		x = 0;
		y = 0;
		for(int k = 0; k < file->getSize(); k++)
		{
			// Compteurs x et y
			if(x == ((spr->image->getImageWidth()*textureScale / 64)) )
			{
				y++;
				x = 0;
			}

			if(file->buffer[k] == '0')
			{
				// Opaque
				meta.data[x][y] = 0;
			}
			else
			{
				// Transparent
				meta.data[x][y] = 1;
			}

			x++;
		}

		tilesets_meta.push_back(meta);
	}
};