#ifndef H_FILELISTER
#define H_FILELISTER

// TODO : non multiplateforme je pense ...
#ifdef _MSC_VER
#include "dirent.h"
#else
#include <dirent.h>
#endif

#include <vector>

std::vector<std::string> listingRecursif(const char* dirname);
int is_dir(char *path);

#endif