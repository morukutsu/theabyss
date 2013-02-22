// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MAP
#define MK_MAP

#include "Drawable.h"
#include "lowDisplay.h"
#include <string>
#include "tmxparser\Tmx.h"
#include <map>
#include <vector>

namespace mk
{
	struct TilesetMetadata
	{
		std::vector<std::vector<int>> data;
	};

	class Sprite;
	class Map
	{
		public:
			Map();
			~Map();

			void SetTextureQuality(int quality);
			void MoveTo(float x, float y);
			void Draw();
			void DrawBGColor();
			void DrawBGImage();
			void DrawLayer(int id, int opaque);
			void SetCullArea(int cx, int cy, int cw, int ch);

			void Load(std::string filename);
			void LoadTilesetMeta(std::string filename, mk::Sprite* spr);

			float GetMapScale() { return mapScale; };

			float GetPosX() { return posX; };
			float GetPosY() { return posY; };
			
			float GetLayerDepth(int id);

			// Attributs
			Tmx::Map map;
			std::vector<mk::Sprite*> tilesets_spr;
			std::vector<TilesetMetadata> tilesets_meta;

		protected:
			float posX, posY;
			float mapScale;
			float textureScale;	// Pour la gestion de la qualité des textures
			int textureQuality;
			u32 bgColor;
			mk::Sprite* backgroundImage;

			// Culling
			int cx, cy, cw, ch;

			// Triggers
			bool isBgImage, isBgColor, isLighting;
	};
};

#endif