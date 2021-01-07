#ifndef TYPE_HEURISTIC_H
#define TYPE_HEURISTIC_H

#include "../external/datum_parser.hpp"
#include <map>

typedef std::map<datum::DataType, std::map<datum::Pattern, int>> PatternCountMap;

class TypeHeuristic
{
public:
	void insert(datum::ParseInstruction const &parse_instruction);
	datum::ParseInstruction get_parse_instruction();

private:
	datum::Pattern get_likely_pattern(datum::DataType const &type);
	datum::DataType get_likely_type();
	PatternCountMap pattern_count_map;
	std::map<datum::DataType, int> type_count;
	void increment_type_count(datum::ParseInstruction parse_instruction);
	void track_pattern_count(datum::ParseInstruction parse_instruction);
};

#endif
