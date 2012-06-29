#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <vector>

/* Useful typedefs for parsing CSV files. */
typedef std::vector<std::string> Row;
typedef std::vector<Row> Table;

/* Returns a table of data that contains whatever is stored in the
 * CSV file with the given name. */
Table ParseCSVFile(const std::string& filename);
void PrintTable(const Table& data);

#endif
