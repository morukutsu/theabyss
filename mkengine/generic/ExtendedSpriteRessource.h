// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_EXTENDEDSPRITERES
#define MK_EXTENDEDSPRITERES

#include <map>
#include <list>
#include <string>
#include <vector>

#include "Image.h"
#include "AsciiFile.h"

#include "Ressource.h"

#include "xml/tinyxml.h"

#include "Matrix.h"

// ExtendedSpriteRessource : Contient les ressources d'un extended sprite
#define DOM_INSTANCE_TYPE_BITMAP		0
#define DOM_INSTANCE_TYPE_MODEL			1

namespace mk 
{
	class DOMNode {
	public: 
		std::string name;
		int node_type;
	};

	class Keyframe {
	public:
		int time;
		Matrix mtx;
		float alpha;
	};

	class DOMInstance {
	public:
		std::string of_name;
		int instance_type;
		DOMNode* of;
		float centerX, centerY;
		float a, b, c, d, tx, ty;

		std::vector<Keyframe> keyframes;
		bool hasMotionObject;
		int fps, duration, animStart;
	};

	class DOMLayer {
	public:
		std::vector<DOMInstance> mInstances;	
	};

	class DOMBitmap : public DOMNode {
	public:
		std::string path;
		Image* bitmap;
	};
	
	class DOMModel : public DOMNode {
	public:
		std::vector<DOMLayer> layers;
	};

	class ExtendedSpriteRessource : public Ressource
	{
		public:
			ExtendedSpriteRessource();
			~ExtendedSpriteRessource();

			int LoadFromFile(const char* filename);		// Loads an ExtendedSpriteRessource from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The ExtendedSpriteRessource pointer must be positionned on the Font
			int LoadFromMemory(char* mem, int size);	// Load an ExtendedSpriteRessource from memory

			DOMNode* FindNodeByName(std::string name);

		private:
			void Parse(AsciiFile& f);

		public:
			// Liste de bitmaps
			std::map<std::string, DOMBitmap> mBitmaps;

			// Liste des models
			std::map<std::string, DOMModel> mModels;
			DOMModel* root;
			
	};
}
#endif