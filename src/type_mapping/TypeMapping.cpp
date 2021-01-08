#include "TypeMapping.hpp"

OutputParseInstruction &TypeMapping::operator[](int const &key)
{
	return mapping.at(headers_map.at(key));
}

OutputParseInstruction &TypeMapping::operator[](std::string const &key)
{
	if (mapping.count(key) == 0)
		throw std::invalid_argument("No type mapping for '" + key + "'");

	return mapping.at(key);
}

TypeMapping TypeMapping::from_headers_sample(ColumnToHeader const &headers_map, SampleRows const &sample_rows)
{
	TypeMapping type_mapping = TypeMapping(headers_map);
	TypeHeuristicMap type_heuristics = TypeHeuristicMap::create(headers_map, sample_rows);

	for (auto [_, header] : headers_map)
		type_mapping.insert(
			header,
			OutputParseInstruction::create(
				type_heuristics.get_parse_instruction(header)));

	return type_mapping;
}

void TypeMapping::insert(std::string const &key, OutputParseInstruction parse_instruction)
{
	mapping.insert({key, parse_instruction});
}

TypeHeuristicMap TypeHeuristicMap::create(ColumnToHeader const &headers_map, SampleRows const &sample_rows)
{
	TypeHeuristicMap type_heuristics;

	for (auto [_, header] : headers_map)
		type_heuristics.heuristic_map[header] = TypeHeuristic();

	for (int row_idx = 0; row_idx < sample_rows.size(); ++row_idx)
	{
		std::vector<std::string> row = sample_rows.at(row_idx);
		for (int column_idx = 0; column_idx < row.size(); ++column_idx)
		{
			std::string header = headers_map.at(column_idx);
			std::string raw_data = row.at(column_idx);
			datum::ParseInstruction parse_instruction = datum::get_parse_instruction(raw_data);
			type_heuristics.heuristic_map[header].insert(parse_instruction);
		}
	}

	return type_heuristics;
}

void TypeHeuristicMap::insert(std::string const &key, TypeHeuristic const &h)
{
	heuristic_map.insert({key, h});
}

datum::ParseInstruction TypeHeuristicMap::get_parse_instruction(std::string const &key)
{
	return heuristic_map.at(key).get_parse_instruction();
}

OutputParseInstruction OutputParseInstruction::create(datum::ParseInstruction const &datum_parse_instruction, std::optional<std::string> const &output_type)
{
	OutputParseInstruction output_parse_instruction = OutputParseInstruction::create(datum_parse_instruction);
	output_parse_instruction.output_type = output_type;
	return output_parse_instruction;
}

OutputParseInstruction OutputParseInstruction::create(datum::ParseInstruction const &datum_parse_instruction)
{
	return OutputParseInstruction(datum_parse_instruction.type, datum_parse_instruction.pattern);
}

datum::ParseInstruction OutputParseInstruction::to_datum_parse_instruction()
{
	return datum::ParseInstruction(type, pattern);
}
