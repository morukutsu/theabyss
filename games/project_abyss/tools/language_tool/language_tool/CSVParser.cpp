#include "CSVParser.h"
#include <stdio.h>
#include <iostream>

CSVParser::CSVParser(std::string filename)
{
	FILE* f = fopen(filename.c_str(), "r");
	if(f == NULL)
	{
		std::cout << "File not found." << std::endl;
		exit(1);
	}

	// Récupération de chaque ligne
	char buf[512];
	char *curStr;
	while(fgets(buf, 512, f) != NULL)
	{
		// Découpage de la ligne en tokens
		CSVRow row;

		curStr = strtok(buf, ";\n");
		while(curStr != NULL)
		{
			std::cout << curStr << std::endl;
			row.cols.push_back(curStr);
			curStr = strtok(NULL, ";\n");
		}

		rows.push_back(row);
	}

	fclose(f);
}

CSVRow& CSVParser::getRow(int id)
{
	return rows[id];
}

int CSVParser::getNumRows()
{
	return rows.size();
}