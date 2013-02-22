// language_tool.cpp : définit le point d'entrée pour l'application console.

#include <iostream>
#include "CSVParser.h"

int main(int argc, char** argv)
{
	// On récupère le fichier passé en paramètres
	if(argc != 2)
	{
		std::cout << "Wrong number of arguments" << std::endl;
		getchar();
		exit(1);
	}

	std::string filename = argv[1];

	std::cout << "Parsing CSV file" << std::endl;

	CSVParser parser(filename);

	// Génération du fichier .h
	std::cout << "Generating .h file" << std::endl;
	//std::string h_name = filename.substr(0, filename.length() - 3) + "h";
	std::string h_name = "loc_header.h";
	std::cout << h_name << std::endl;
	FILE* h_file = fopen(h_name.c_str(), "w+");
	if(h_file == NULL)
	{
		std::cout << "Error in .h generation" << std::endl;
		getchar();
		exit(1);
	}

	fprintf(h_file, "// language file\n");
	fprintf(h_file, "#ifndef _H_LANG_HEADER\n");
	fprintf(h_file, "#define _H_LANG_HEADER\n");
	for(int i = 1; i < parser.getNumRows(); i++)
	{
		fprintf(h_file, "#define LOC_%s %d\n", parser.getRow(i).cols[0].c_str(), i);
	}
	fprintf(h_file, "#endif\n");

	fclose(h_file);

	// Génération fichier XML
	std::string xml_name = filename.substr(0, filename.length() - 3) + "xml";
	FILE* xml_file = fopen(xml_name.c_str(), "w+");
	if(xml_file == NULL)
	{
		std::cout << "Error in .xml generation" << std::endl;
		getchar();
		exit(1);
	}

	fprintf(xml_file, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	fprintf(xml_file, "<Language>\n");
	for(int i = 1; i < parser.getNumRows(); i++)
	{
		fprintf(h_file, "<Str id=\"%d\">%s</Str>\n", i, parser.getRow(i).cols[1].c_str());
	}
	fprintf(xml_file, "</Language>\n");
	fclose(xml_file);


	return 0;
}
