// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_IMAGE
#define MK_IMAGE

#include <SFML/Graphics.hpp>
#include "Ressource.h"
#include "gctypes.h"

// Image : platform specific image handling
namespace mk 
{
	enum { IMG_FROM_FILE, IMG_FROM_RESSOURCE_MANAGER };
	enum { IMG_OK, IMG_FILE_NOT_FOUND, IMG_ALLOC_ERROR, IMG_PNG_ERROR };

	class Image : public Ressource
	{
		public:
			Image();
			~Image();
			int LoadFromFile(const char* filename);		// Loads a png image from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The file pointer must be positionned on the png image
			int LoadFromMemory(char* mem, int size);	// Load a png from memory
			int Create(int w, int h);
			void Free();

			void Bind();								// Binds the image to the GPU current context
			int getImageWidth();
			int getImageHeight();
			int getLoadSource();
			int getUsedMemory();

			u32 getPixel(int x, int y) {
				return (u32)(0);
			}
			
			void setPixel(int x, int y, u32 color) {
			}

			void *getDataPtr();							// Dangerous (not crossplatform)
			void Flush();

		public:
			int mLoadedFrom, mImageWidth, mImageHeight, mUsedMemory;
			int mImageColorMode;						// 0 : RGB, 1 : Grayscale

			//Platform specific data
			sf::Texture img;
	};
};

#endif