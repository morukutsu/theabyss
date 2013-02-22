#include <string>
#include <vector>

class CSVRow {
public:
	std::vector<std::string> cols;
};

class CSVParser {
	public:
		CSVParser(std::string filename);

		CSVRow& getRow(int id);
		int getNumRows();

	public:
		std::vector<CSVRow> rows;
};