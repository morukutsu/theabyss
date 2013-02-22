// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_ASSETS_PREPROCESSING
#define MK_ASSETS_PREPROCESSING

#include "singleton.h"
#include <string>

namespace mk
{
	class AssetsPreprocessing : public Singleton<AssetsPreprocessing> 
	{
		friend class Singleton<AssetsPreprocessing>;

		public:
			// Génération des .txt des tilesets, permettant de déterminer quels sont les tiles transparents
			void GenerateTilesetMetafile(std::string tileset_filename, std::string target_filename);
			void GenerateLowQualityTextures(std::string tex_filename);

		public:
	};
};

#endif