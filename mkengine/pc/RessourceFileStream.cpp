// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "RessourceFileStream.h"
#include "RessourceManager.h"

RessourceFileStream::RessourceFileStream() :
m_file(NULL)
{
}

RessourceFileStream::~RessourceFileStream()
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
			std::fclose(m_file);
	}
}

bool RessourceFileStream::open(const std::string& filename)
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
			std::fclose(m_file);

		m_file = std::fopen(("data/" + filename).c_str(), "rb");
	}
	else
	{
		mTocInfo = mk::RessourceManager::getInstance()->GetRessourceInfos(filename);
		
		m_file = mk::RessourceManager::getInstance()->mArchive;
		fseek(m_file, mTocInfo->offset, SEEK_SET);
		filePosition = 0;
	}

    return m_file != NULL;
}

sf::Int64 RessourceFileStream::read(void* data, sf::Int64 size)
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
			return std::fread(data, 1, static_cast<std::size_t>(size), m_file);
		else
			return -1;
	}
	else
	{
		fseek(m_file, mTocInfo->offset + filePosition, SEEK_SET);

		std::size_t will_read = std::min((unsigned long)(mTocInfo->size - filePosition), (unsigned long)static_cast<std::size_t>(size));
		std::size_t read_bytes = std::fread(data, 1, static_cast<std::size_t>(size), m_file);
		filePosition += read_bytes;	
		return read_bytes;
	}
}

sf::Int64 RessourceFileStream::seek(sf::Int64 position)
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
		{
			std::fseek(m_file, static_cast<std::size_t>(position), SEEK_SET);
			return tell();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if (m_file)
		{
			std::fseek(m_file, static_cast<std::size_t>(mTocInfo->offset + position), SEEK_SET);
			filePosition = position;
			return tell();
		}
		else
		{
			return -1;
		}
	}
}

sf::Int64 RessourceFileStream::tell()
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
			return std::ftell(m_file);
		else
			return -1;
	}
	else
	{
		return filePosition;
	}
}

sf::Int64 RessourceFileStream::getSize()
{
	if(!mk::RessourceManager::getInstance()->mLoadingMode)
	{
		if (m_file)
		{
			sf::Int64 position = tell();
			std::fseek(m_file, 0, SEEK_END);
			sf::Int64 size = tell();
			seek(position);
			return size;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return mTocInfo->size;
	}
}