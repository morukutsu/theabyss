// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_RESSOURCE_FILE_STREAM
#define H_RESSOURCE_FILE_STREAM

#include <SFML/System.hpp>
#include <string>
#include <cstdio>

#include "../generic/RessourceManager.h"

class RessourceFileStream : public sf::InputStream
{
public :

    RessourceFileStream();

    ~RessourceFileStream();

    bool open(const std::string& filename);

    virtual sf::Int64 read(void* data, sf::Int64 size);

    virtual sf::Int64 seek(sf::Int64 position);

    virtual sf::Int64 tell();

    virtual sf::Int64 getSize();

private :
    std::FILE* m_file;
	mk::sArchiveFile* mTocInfo;
	size_t filePosition;
};

#endif