#ifndef TYPE_HEURISTIC_MAP_H
#define TYPE_HEURISTIC_MAP_H

#include <map>
#include <vector>
#include "TypeHeuristic.hpp"
#include "../external/datum_parser.hpp"

typedef std::map<unsigned int, std::string> ColumnToHeader;
typedef std::vector<std::vector<std::string>> SampleRows;
typedef std::optional<std::string> OutputType;

class TypeHeuristicMap
{
public:
	static TypeHeuristicMap create(ColumnToHeader const &headers_map, SampleRows const &sample_rows);
	datum::ParseInstruction get_parse_instruction(std::string const &key);

private:
	void insert(std::string const &key, TypeHeuristic const &h);
	std::map<std::string, TypeHeuristic> heuristic_map;
	TypeHeuristicMap() {}
};

#endif
