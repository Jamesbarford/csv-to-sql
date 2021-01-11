#include "TypeHeuristicMap.hpp"

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
