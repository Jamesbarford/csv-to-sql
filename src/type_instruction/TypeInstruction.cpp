#include "TypeInstruction.hpp"

TypeInstruction &TypeMap::operator[](std::string const &key)
{
	return type_map[key];
}

// TypeInstruction("start_date", "TIMESTAMP", 0, datum::DataType::Date, "%Y-%m-%d")
TypeMapping TypeMap::to_type_mapping()
{
	ColumnToHeader header_map;
	OutputParseInstructionMap mapping;

	for (auto [header, type_instruction] : type_map)
	{
		std::string output_type = std::get<1>(type_instruction);
		unsigned int column_index = std::get<2>(type_instruction);
		datum::DataType datum_type = std::get<3>(type_instruction);
		datum::Pattern datum_pattern = std::get<4>(type_instruction);

		OutputParseInstruction parse_instruction = OutputParseInstruction(datum_type, datum_pattern, output_type);

		header_map.insert({column_index, header});
		mapping.insert({header, parse_instruction});
	}

	return TypeMapping(header_map, mapping);
}
