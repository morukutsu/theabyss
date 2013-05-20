// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "RessourceManager.h"
#include "Image.h"
#include "Font.h"
#include "MeshBone.h"
#include "MeshBoneAnim.h"
#include "AsciiFile.h"
#include "ExtendedSpriteRessource.h"
#include "lowSystem.h"
#include "Shader.h"

namespace mk
{
    RessourceManager::RessourceManager()
    {
        resList.clear();
		mLoadJobs.clear();
		mLoadingMode = false;
		chksum_crc32gentab();

		//Création du thread de chargement
		lowRunThread(&sysLoadingThread, NULL);

		// Prefixe de chargement des fichiers
		mFileLoadingPrefix = "data/";

		mVramUsage = 0;

		mArchive = NULL;
    }

    RessourceManager::~RessourceManager()
    {
        resList.clear();
    }

	void RessourceManager::LoadTableOfContents(string filename)
	{
		//Ouverture du fichier archive
		mArchive = fopen(filename.c_str(), "rb");
		if(mArchive)
		{
			//Chargement de la TOC
			int fNum = 0;
			fread(&fNum, sizeof(int), 1, mArchive);
			fNum = lowConvertInt(fNum);
			for(int i = 0; i < fNum; i++)
			{
				sArchiveFile arcf;
				//Hash, taille, offset
				fread(&arcf.hash, sizeof(int), 1, mArchive);
				fread(&arcf.size, sizeof(long), 1, mArchive);
				fread(&arcf.offset, sizeof(long), 1, mArchive);
				arcf.hash = lowConvertInt(arcf.hash);
				arcf.size = lowConvertInt(arcf.size);
				arcf.offset = lowConvertInt(arcf.offset);
	
				mToc[arcf.hash] = arcf;
			}
		}
		mLoadingMode = true;
	}

	long RessourceManager::GetCurrentPosition()
	{
		if(mArchive)
		{
			return ftell(mArchive);
		}
		else
		{
			return 0;
		}
	}

	void RessourceManager::SetCurrentPosition(long p)
	{
		if(mArchive)
		{
			fseek(mArchive, p, SEEK_SET);
		}
	}

	Ressource* RessourceManager::LoadRessource(string filename)
	{
		//Hashing du filename
		//std::cout << filename << std::endl;
		unsigned int hash = chksum_crc32((unsigned char*)filename.c_str(), strlen(filename.c_str()));
		Ressource* tmpRes = NULL;

        //Verification de l'existance possible de la ressource
        //Iterateur
        for(RessourceList::iterator i = resList.begin(); i!=resList.end(); ++i)
        {
            if(i->first == hash)
            {
                //printf("[Notif] Image deja chargée (%s)\n	", filename.c_str() );
                return i->second;
            }
        }

		//Chargement depuis le fichier ou depuis l'archive
		FILE* mFilePointer;
		unsigned long mFileSize = 0;

		if(mLoadingMode == false)
		{
			std::string loadingString = mFileLoadingPrefix + filename;
			mFilePointer = fopen(loadingString.c_str(), "rb");
			
			if(mFilePointer == NULL)
				lowError("File not found : " + loadingString);

			// On récupère la taille du fichier
			fseek(mFilePointer, 0, SEEK_END);
			mFileSize = ftell(mFilePointer);
			fseek(mFilePointer, 0, SEEK_SET);
		}
		else
		{
			//Positionnement du fichier
			mFilePointer = mArchive;
			mFileSize = mToc[hash].size;
			fseek(mFilePointer, mToc[hash].offset, SEEK_SET);
		}

        //Chargement depuis le fichier
		if(mFilePointer)
		{
			//Récupération de l'extention
			istringstream iss(filename);
			string ext;
			while(std::getline(iss, ext, '.' ) )
			{
			}

			//On a la dernière chaine après le séparateur

			//On détermine la méthode de chargement à utiliser en fonction de l'extension
			//string ext = filename.substr(filename.size()-3, 3);
			if(ext == "png")
				tmpRes = new Image();
			else if(ext == "fnt")
				tmpRes = new Font();
			else if(ext == "md5mesh")
				tmpRes = new MeshBone();
			else if(ext == "md5anim")
				tmpRes = new MeshBoneAnim();
			else if(ext == "xml" || ext == "tmx" || ext == "ogg" || ext == "txt" || ext == "json" || ext == "atlas" || ext == "as")
				tmpRes = new AsciiFile();
			else if(ext == "raw")
				tmpRes = new Sound();
			else if(ext == "spr")
				tmpRes = new ExtendedSpriteRessource();
			else if(ext == "fx")
				tmpRes = new Shader();

			tmpRes->LoadFromFile(mFilePointer, mFileSize);
			tmpRes->mFromRessourceManager = true;
			resList[hash] = tmpRes;

			if(mLoadingMode == false)
				fclose(mFilePointer);

			std::cout << "[Load] : " << hash << " | " << filename << std::endl;
		}
		
		return tmpRes;
    }

	void RessourceManager::LoadRessourceAsync(string filename)
	{
		//Ajoute des fichiers à la charger à la liste de chargement
		mLoadJobs.push_back(filename);
	}

	//Commence à enregistrer une liste de ressources à charger
	void RessourceManager::StartRecording()
	{
		recording.clear();
		isRecording = true;
	}

	//Retourne la liste des ressources enregistrées
	std::vector<Ressource*> RessourceManager::EndRecording()
	{
		isRecording = false;
		return recording;
	}

	
    void RessourceManager::DeleteRessource(Ressource* res)
    {
        //Iterateur
		for (RessourceList::iterator i = resList.begin(); i!=resList.end(); ++i)
		{
			if (i->second == res)
			{
				std::cout << "Deleted : " << i->first << std::endl;
				delete i->second;
				resList.erase(i);
				break;
			}
		}
    }

	void RessourceManager::DeleteRessource(string filename)
	{
		// Récupération de la ressource
		unsigned int hash = chksum_crc32((unsigned char*)filename.c_str(), strlen(filename.c_str()));

		// Iterateur
		for (RessourceList::iterator i = resList.begin(); i!=resList.end(); ++i)
		{
			if (i->first == hash)
			{
				std::cout << "Deleted : " << i->first << std::endl;
				delete i->second;
				resList.erase(i);
				break;
			}
		}
	}

    void RessourceManager::Free()
    {
        //Iterateur
        for (RessourceList::iterator i = resList.begin(); i != resList.end(); )
        {
			std::cout << "Deleted : " << i->first << std::endl;
			delete (i->second);
            resList.erase(i++);
        }    
    }

};

#ifdef GEKKO
void* sysLoadingThread(void* data)
#else
void sysLoadingThread(void* data)
#endif
{
	//Récupération de l'instance du ressource manager
	mk::RessourceManager* rman = mk::RessourceManager::getInstance();

	//Lancement de la boucle de chargement threadé
	while(1)
	{
		//Parcours de la liste des ressources à charger
		if(rman->mLoadJobs.size() > 0)
		{
			for(std::list<std::string>::iterator i = rman->mLoadJobs.begin(); i != rman->mLoadJobs.end(); i++)
			{
				rman->LoadRessource((*i));
				std::cout << "[Thread] Loaded " << (*i) << std::endl;
			}
			rman->mLoadJobs.clear();
		}
		else
		{
			//Idle
			lowThreadWait();
		}
	}
};
