#include "TypeInstruction.hpp"

std::string datum_type_to_sqlite_type(datum::DataType const &type);

void TypeInstructionMap::insert(TypeInstruction const &type_instruction)
{
	unsigned int column_index = std::get<2>(type_instruction);
	std::string header = std::get<0>(type_instruction);

	column_to_header[column_index] = header;
	type_map[header] = type_instruction;
}

TypeInstruction TypeInstructionMap::at(unsigned int const &column_index)
{
	std::string header = column_to_header[column_index];
	return type_map[header];
}

/* the order is defined by the column to header map which is keyed by the column index */
void TypeInstructionMap::for_each(std::function<void(std::string const &header, TypeInstruction const &instruction)> iteratee)
{
	for (auto [column_index, header] : column_to_header)
		iteratee(header, type_map[header]);
}

unsigned int TypeInstructionMap::size()
{
	return type_map.size();
}

TypeInstructionMap TypeInstructionMap::__from_headers_sample(ColumnToHeader const &headers_map, SampleRows const &sample_rows)
{
	TypeInstructionMap type_instruction_map;
	TypeHeuristicMap type_heuristics = TypeHeuristicMap::create(headers_map, sample_rows);

	for (auto [column_idx, header] : headers_map)
	{
		datum::ParseInstruction parse_instruction = type_heuristics.get_parse_instruction(header);
		type_instruction_map.insert(TypeInstruction(
			header,
			datum_type_to_sqlite_type(parse_instruction.type),
			column_idx,
			parse_instruction.type,
			parse_instruction.pattern));
	}

	return type_instruction_map;
}

datum::DataType raw_to_datum_type(std::string const &raw_type)
{
	std::map<std::string, datum::DataType> raw_to_datum_type = {
		{"string", datum::DataType::String},
		{"date", datum::DataType::Date},
		{"float", datum::DataType::Float},
		{"int", datum::DataType::Integer},
		{"currency", datum::DataType::Currency},
		{"percentage", datum::DataType::Percentage}};

	if (raw_to_datum_type.count(raw_type) <= 0)
	{
		std::cout << "Raw type : '" << raw_type << "' is invalid defaulting to type 'String'\n";
		return datum::DataType::String;
	}

	return raw_to_datum_type[raw_type];
}

std::string datum_type_to_sqlite_type(datum::DataType const &type)
{
	switch (type)
	{
	case datum::DataType::Currency:
	case datum::DataType::Float:
	case datum::DataType::Percentage:
		return "REAL";

	case datum::DataType::Integer:
		return "INTEGER";

	case datum::DataType::Date:
	case datum::DataType::String:
		return "TEXT";

	default:
		break;
	}
}
