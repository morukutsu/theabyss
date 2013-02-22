// filepacker.cpp : définit le point d'entrée pour l'application console.
// --------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include "dirent.h"
#include <string>
#include "crc32.h"

//Structure du fichier ressource.bin; Nombre de fichiers - TOC(hash, offset, size) - Fichiers
struct sArchiveFile
{
	unsigned int hash;
	long offset, size;
	char realFileName[512];
};

std::map<unsigned int, sArchiveFile> mToc;

void listingRecursif(const char* dir);
const char* strRemovePrefix(const char* str);
char* prefix;
int fsize(const char * fname, long * ptr);
int is_dir(char *path);
void CopieFichierT(FILE *cible, FILE *source, long taille);

//Point d'entrée
int main(int argc, char* argv[])
{
	//Parcours tout les dossiers ou se trouve l'executable
	chksum_crc32gentab();
	prefix = argv[1];
	listingRecursif(argv[1]);

	std::cout << "MK Engine File Packer" << std::endl;
	std::cout << "Pack all assets in one file to speedup install/loadings" << std::endl;

	//Création de l'archive
	if(argc < 2)
	{
		std::cout << "no folder given" << std::endl;
		exit(1);
	}

	
	std::cout << "Building game assets..." << std::endl;

	FILE* f = fopen("ressource.bin", "wb+");
	if(f)
	{
		//-- Nombre de fichiers
		int size = mToc.size();
		fwrite(&size, sizeof(int), 1, f);

		//-- TOC
		long currentOffset = sizeof(int) + size*(sizeof(int) + 2*sizeof(long)); //Positionnement a la fin de la TOC
		for(std::map<unsigned int, sArchiveFile>::iterator it = mToc.begin(); it != mToc.end(); it++)
		{
			//Hash, taille, offset
			fwrite(&it->second.hash, sizeof(int), 1, f);
			fwrite(&it->second.size, sizeof(long), 1, f);
			fwrite(&currentOffset, sizeof(long), 1, f);

			currentOffset += it->second.size;
		}

		//-- Ecriture des fichiers
		for(std::map<unsigned int, sArchiveFile>::iterator it = mToc.begin(); it != mToc.end(); it++)
		{
			//Ouverture du fichier concerné
			FILE* src = fopen(it->second.realFileName, "rb");
			if(src)
			{
				CopieFichierT(f, src, it->second.size);
				fclose(src);
			}
		}
		fclose(f);
	}

	getchar();
	return 0;
}

void listingRecursif(const char* dirname)
{
	DIR *folder;
	struct dirent *entity;
	char *real_path=NULL;
	struct stat statbuf;

    folder=opendir(dirname);

	if(folder==NULL)
		return;
	
	while (entity=readdir(folder))
	{
		stat(entity->d_name, &statbuf);
		if(real_path!=NULL)
		{
			free(real_path);
			real_path=NULL;
		}

		if(!strcmp(entity->d_name,".") || !strcmp(entity->d_name,"..") ) //Evite quelques problemes sous Unix/Linux
			continue;
		
		real_path=(char *)malloc( strlen(dirname) + strlen(entity->d_name) + 2);/* Si c'est+1 au lieu de +2, j'ai parfois un problème de segmentation. Sais pas pourquoi .. */
		
		sprintf(real_path,"%s/%s",dirname,entity->d_name);
		
		if( is_dir(real_path) == 1 )
			listingRecursif(real_path);
		else
		{
			//On a récupéré le nom du fichier
            printf("Fichier: %s\n", strRemovePrefix(real_path) );

			//Création d'une structure pour le contenir
			sArchiveFile arcf;
			strcpy(arcf.realFileName, real_path);
			arcf.hash = chksum_crc32((unsigned char*)strRemovePrefix(real_path), strlen((const char*)strRemovePrefix(real_path)));
			fsize(real_path, &arcf.size);

			mToc[arcf.hash] = arcf;

			printf("--- h:%ld size:%ld\n", arcf.hash, arcf.size);
		}
	}

	closedir(folder);

	if (real_path!=NULL)
	{
		free(real_path);
		real_path=NULL;
	}
			
	return;
}

const char* strRemovePrefix(const char* str)
{
	const char* noprefix;
	noprefix = str+strlen(prefix)+1;
	return noprefix;
}

int is_dir(char *path)
{
	DIR *is_folder=NULL;

	is_folder=opendir(path);
	if (is_folder!=NULL){
		closedir(is_folder);
		return 1;
	}	
	
	closedir(is_folder);
	return 0;	
		
}

int fsize(const char * fname, long * ptr)
{
    /* Cette fonction retourne 0 en cas de succes, une valeur differente dans le cas contraire. */
    /* La taille du fichier, si elle a pu etre calculee, est retournee dans *ptr                */
    
    FILE * f;
    int ret = 0;
    
    f = fopen(fname, "rb");   
    if (f != NULL)
    {
        fseek(f, 0, SEEK_END); /* aller a la fin du fichier */
        *ptr = ftell(f); /* lire l'offset de la position courante par rapport au debut du fichier */
        fclose(f);
    }
    else
        ret = 1;
    
    return ret;
}

void CopieFichierT(FILE *cible, FILE *source, long taille)
{
    char buffer[4096];
    long octetsLus, octetsALire;

    octetsALire = taille;
    octetsLus = 0;

    while(octetsALire > 0)
    {
        //Remplissage du buffer
		long toRead = octetsALire>4096?4096:octetsALire;

        octetsLus = fread(buffer, sizeof(char), toRead, source);
        fwrite(buffer, sizeof(char), toRead, cible);
        octetsALire -= octetsLus;
    }
}