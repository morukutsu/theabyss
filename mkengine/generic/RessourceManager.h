// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_RESSOURCEMANAGER
#define MK_RESSOURCEMANAGER

#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include "singleton.h"
#include "Ressource.h"
#include "crc32.h"
#include <vector>

//Fonction de chargement thread�e
#ifdef GEKKO
	void* sysLoadingThread(void* data);	
#else
	void sysLoadingThread(void* data);
#endif


// Ressource manager
namespace mk
{
    class Image;
    using namespace std;
    typedef map<unsigned int, Ressource*> RessourceList;

	struct sArchiveFile
	{
		unsigned int hash;
		long offset, size;
	};

    class RessourceManager : public Singleton<RessourceManager>
    {
        friend class Singleton<RessourceManager>;

    public:
        RessourceManager();                          //Constructeur
        ~RessourceManager();                         //Destructeur
		
		void LoadTableOfContents(string filename);   //Charge le fichier archive
		long GetCurrentPosition();
		void SetCurrentPosition(long p);

        Ressource* LoadRessource(string filename);	 //Fonction de chargement de ressource
		void DeleteRessource(Ressource* res);		 //Suppression de ressource  
		void Free();                                 //On vide tout le manager

		void LoadRessourceAsync(string filename);	//Fonction de chargement de ressource thread�e
		void DeleteRessourceAsync(Ressource* res);  //Fonction de d�chargement de ressource thread�e
		void FreeAsync();							//Vide tout le manager dans un thread. Utile ?
		
		void StartRecording();						//Commence � enregistrer une liste de ressources � charger
		std::vector<Ressource*> EndRecording();		//Retourne la liste des ressources enregistr�es

		std::list<string> mLoadJobs;
		std::list<Ressource*> mFreeJobs;

		bool mLoadingMode;							 //Si la TOC est charg�e, on charge depuis le .bin
		
		// Statistiques
		long mVramUsage;							// Vram occup�e en octets
   
	private:
		std::map<unsigned int, sArchiveFile> mToc;	
		std::string mFileLoadingPrefix;

        RessourceList resList;
		
		FILE* mArchive;

		std::vector<Ressource*> recording;
		bool isRecording;
    };
};


#endif
