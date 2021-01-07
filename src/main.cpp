#include <iostream>
#include "csv_to_sql.hpp"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr
			<< "Invalid argument count\n"
			<< "arg 1 must be the path to the csv \n"
			<< "arg 2 is the output destination name i.e ./dir/table.sql \n";
	}

	std::string query = csv_to_sql(argv[1], argv[2]);
	write_to_file(query, argv[2]);
}
