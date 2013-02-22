#include "FileLister.h"
#include <string>
#include <vector>
#include <stdio.h>

std::vector<std::string> listingRecursif(const char* dirname)
{
	std::vector<std::string> filenames;

	DIR *folder;
	struct dirent *entity;
	char *real_path=NULL;
	struct stat statbuf;

    folder=opendir(dirname);

	if(folder==NULL)
		return filenames;
	
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
			// Ajout a la liste de fichiers
			filenames.push_back(real_path+5);
		}
	}

	closedir(folder);

	if (real_path!=NULL)
	{
		free(real_path);
		real_path=NULL;
	}
			
	return filenames;
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