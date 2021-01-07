#include <iostream>
#include "csv_to_sql.hpp"

int main(void)
{
	std::string query = csv_to_sql("./test-csv/debug.csv", "debug");
	write_to_file(query, "debug");
}
