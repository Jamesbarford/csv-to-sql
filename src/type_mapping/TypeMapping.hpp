#ifndef TYPE_MAP_H
#define TYPE_MAP_H

#include <map>
#include <vector>
#include "TypeHeuristic.hpp"
#include "../external/datum_parser.hpp"

typedef std::map<unsigned int, std::string> ColumnToHeader;
typedef std::vector<std::vector<std::string>> SampleRows;

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

class TypeMapping
{
public:
	datum::ParseInstruction &operator[](int const &key);
	datum::ParseInstruction &operator[](std::string const &key);
	static TypeMapping from_headers_sample(ColumnToHeader const &headers_map, SampleRows const &sample_rows);
	void insert(std::string const &key, datum::ParseInstruction parse_instruction);
	ColumnToHeader headers_map;

private:
	std::map<std::string, datum::ParseInstruction> mapping;
};

#endif
