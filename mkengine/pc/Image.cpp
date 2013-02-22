// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Image.h"
#include <iostream>
#include "RessourceManager.h"

// Image : platform specific image handling
namespace mk 
{
	Image::Image()
	{
		mImageWidth = mImageHeight = mLoadedFrom = mUsedMemory = -1;
		mImageColorMode = 0;
	}

	Image::~Image()
	{
		mk::RessourceManager::getInstance()->mVramUsage -= mUsedMemory;
	}

	int Image::LoadFromFile(const char* filename)
	{
		if(img.loadFromFile(filename) )
		{
			mImageWidth = img.getSize().x;
			mImageHeight = img.getSize().y;
			mLoaded = true;
	
			mUsedMemory = mImageHeight*mImageWidth*8;
			mk::RessourceManager::getInstance()->mVramUsage += mUsedMemory;
			return IMG_OK;
		}
		else
		{
			return IMG_FILE_NOT_FOUND;
		}
	}

	int Image::LoadFromFile(FILE* fp, long size)
	{
		//Sur PC, on charge le fichier entièrement en mémoire
		char* buffer = (char*)malloc(size);
		fread(buffer, sizeof(char), size, fp);
		
		//Chargement de l'image
		img.loadFromMemory(buffer, size);
		
		mImageWidth = img.getSize().x;
		mImageHeight = img.getSize().y;

		mUsedMemory = mImageHeight*mImageWidth*8;
		mk::RessourceManager::getInstance()->mVramUsage += mUsedMemory;

		free(buffer);
		mLoaded = true;
		return IMG_OK;
	}

	int Image::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	int Image::Create(int w, int h)
	{
		img.create(w, h);
		mImageWidth = w;
		mImageHeight = h;
		mLoaded = true;

		mUsedMemory = mImageHeight*mImageWidth*8;
		mk::RessourceManager::getInstance()->mVramUsage += mUsedMemory;

		return IMG_OK;
	}

	void Image::Free()
	{
		
	}

	void Image::Bind()
	{
		sf::Texture::bind(&img);
	}
			
	int Image::getImageWidth()
	{
		return mImageWidth;
	}

	int Image::getImageHeight()
	{
		return mImageHeight;
	}

	int Image::getLoadSource()
	{
		return mLoadedFrom;
	}

	int Image::getUsedMemory()
	{
		return mUsedMemory;
	}

	void* Image::getDataPtr()
	{
		// ne plus utiliser ...
		return (void*)0;
	}

	void Image::Flush()
	{

	}
};