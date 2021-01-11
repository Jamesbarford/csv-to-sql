#include "TypeHeuristic.hpp"

/* Will keep track of DataType count and a count of patterns to make an educated guess at parsing */
void TypeHeuristic::insert(datum::ParseInstruction const &parse_instruction)
{
	increment_type_count(parse_instruction);
	track_pattern_count(parse_instruction);
}

void TypeHeuristic::increment_type_count(datum::ParseInstruction parse_instruction)
{
	type_count.count(parse_instruction.type) == 1 ? type_count[parse_instruction.type]++ : type_count[parse_instruction.type] = 0;
}

void TypeHeuristic::track_pattern_count(datum::ParseInstruction parse_instruction)
{
	// we've been here
	if (pattern_count_map.count(parse_instruction.type) == 1)
	{
		std::map<datum::Pattern, int> *pattern_c = &pattern_count_map[parse_instruction.type];
		if (pattern_c->count(parse_instruction.pattern) == 1)
			pattern_c->at(parse_instruction.pattern)++;
		else
			pattern_c->at(parse_instruction.pattern) = 0;
	}
	else
		pattern_count_map[parse_instruction.type][parse_instruction.pattern] = 0;
}

datum::ParseInstruction TypeHeuristic::get_parse_instruction()
{
	datum::DataType likely_type = get_likely_type();
	datum::Pattern likely_pattern = get_likely_pattern(likely_type);
	return datum::ParseInstruction(likely_type, likely_pattern);
}

datum::Pattern TypeHeuristic::get_likely_pattern(datum::DataType const &type)
{
	unsigned short acc = 0;
	datum::Pattern likely_pattern = "";

	for (auto [pattern, count] : pattern_count_map[type])
	{
		if (count > acc)
		{
			likely_pattern = pattern;
			acc = count;
		}
	}

	return likely_pattern;
}

datum::DataType TypeHeuristic::get_likely_type()
{
	unsigned short acc = 0;
	datum::DataType likely_type = datum::DataType::String;

	for (auto [type, count] : type_count)
	{
		if (count > acc)
		{
			likely_type = type;
			acc = count;
		}
	}

	return likely_type;
}
