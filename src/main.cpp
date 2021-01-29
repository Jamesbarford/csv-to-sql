#include <iostream>

#include "csv_to_sql.hpp"
#include "./external/json.hpp"
#include "./type_instruction/TypeInstruction.hpp"

int main(int argc, char *argv[])
{
	using json = nlohmann::json;

	if (argc < 4)
	{
		std::cerr
			<< "Invalid argument count\n"
			<< "arg 1: path to the csv \n"
			<< "arg 2: table name \n"
			<< "arg 3: JSON schema \n";
		exit(1);
		return 1;
	}

	json json_schema = json::parse(std::ifstream(argv[3]));

	TypeInstructionMap type_instruction_map;

	for (auto [key, value] : json_schema.items())
	{
		if (value.size() < 3)
		{
			std::cerr << "JSON array needs to contain 3 values minimum\n"
					  << "Required: \n"
					  << "0: SQL type\n"
					  << "1: column index of csv where value occurs\n"
					  << "2: raw data type \n"
					  << "Optional: \n"
					  << "3: date pattern i.e %Y-%m-%d\n";
			exit(1);
			return 1;
		}

		type_instruction_map.insert(TypeInstruction(
			key,
			value.at(0).get<std::string>(),
			value.at(1).get<unsigned int>(),
			raw_to_datum_type(value.at(2).get<std::string>()),
			value.size() == 4 ? value.at(3).get<std::string>() : ""));
	}

	std::cout << "Generating SQL file..." << '\n';
	csv_to_sql_file(argv[1], argv[2], type_instruction_map);
	std::cout << argv[2] << ".sql successfully created" << '\n';
}
