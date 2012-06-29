#include <fstream>
#include <iostream>

#include "CSVLoader.h"

Table ParseCSVFile(const std::string& filename) {
    // Stores parsed data
	Table data;
	// Temporary storage - used to store value of current
    // field and value(s) of current row being parsed
	Row currentRow;
	currentRow.reserve(50);
	std::string buffer;
	buffer.reserve(300);

    // Open an input stream to the file with the given name
	std::ifstream file(filename.c_str());
	// Keep reading characters from the file until we reach
	// the end or an IO error occurs
	while (file.good()) {
		char ch = file.get();
		if (ch == ',')
		{
			currentRow.push_back(buffer);
			buffer.clear();
		}
		else if (ch == '\n')
		{
			currentRow.push_back(buffer);
			buffer.clear();

			data.push_back(currentRow);
			currentRow.clear();
		}
		else
		{
			buffer.push_back(ch);
		}
	}

	return data;
}

void PrintTable(const Table& data)
{
	for (unsigned int i = 0; (i < data.size()); ++i)
	{
		const Row& row = data[i];
		for (unsigned int j = 0; (j < row.size()); ++j)
		{
			std::cout << row[j] << ",";
		}
		std::cout << std::endl;
	}
}
