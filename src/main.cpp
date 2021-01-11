#include <iostream>
#include "csv_to_sql.hpp"
#include "./type_instruction/TypeInstruction.hpp"

// "row_number", "uuid", "start_date", "end_date", "uuid_2"
int main(int argc, char *argv[])
{
	// if (argc < 3)
	// {
	// 	std::cerr
	// 		<< "Invalid argument count\n"
	// 		<< "arg 1 must be the path to the csv \n"
	// 		<< "arg 2 is the output destination name i.e ./dir/table.sql \n";
	// }

	TypeInstructionMap type_instruction_map;

	type_instruction_map.insert(TypeInstruction("row_number", "INTEGER", 0, datum::DataType::Integer));
	type_instruction_map.insert(TypeInstruction("uuid", "TEXT", 1, datum::DataType::String));
	type_instruction_map.insert(TypeInstruction("start_date", "TIMESTAMP", 2, datum::DataType::Date, "%Y-%m-%d"));
	type_instruction_map.insert(TypeInstruction("end_date", "TIMESTAMP", 3, datum::DataType::Date, "%Y-%m-%d"));
	type_instruction_map.insert(TypeInstruction("uuid_2", "TEXT", 4, datum::DataType::String));

	std::string query = csv_to_sql("./test-csv/2thou.csv", "2thou", type_instruction_map);
	write_to_file(query, "2thou.sql");
}
