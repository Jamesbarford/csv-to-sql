#ifndef TYPE_MAP_H
#define TYPE_MAP_H

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

class OutputParseInstruction : public datum::ParseInstruction
{
public:
	OutputParseInstruction(datum::DataType d_t) : datum::ParseInstruction(d_t){};
	OutputParseInstruction(datum::DataType d_t, datum::Pattern p) : datum::ParseInstruction(d_t, p){};
	OutputParseInstruction(datum::DataType d_t, datum::Pattern p, std::optional<std::string> o) : datum::ParseInstruction(d_t, p), output_type(o){};
	static OutputParseInstruction create(datum::ParseInstruction const &datum_parse_instruction);
	static OutputParseInstruction create(datum::ParseInstruction const &datum_parse_instruction, std::optional<std::string> const &output_type);
	datum::ParseInstruction to_datum_parse_instruction();
	std::optional<std::string> output_type;
};

typedef std::map<std::string, OutputParseInstruction> OutputParseInstructionMap;

class TypeMapping
{
public:
	TypeMapping();
	TypeMapping(ColumnToHeader h_m) : headers_map(h_m){};
	TypeMapping(ColumnToHeader h_m, OutputParseInstructionMap m) : headers_map(h_m), mapping(m){};
	OutputParseInstruction &operator[](int const &key);
	OutputParseInstruction &operator[](std::string const &key);
	static TypeMapping from_headers_sample(ColumnToHeader const &headers_map, SampleRows const &sample_rows);
	void insert(std::string const &key, OutputParseInstruction parse_instruction);
	ColumnToHeader headers_map;

private:
	OutputParseInstructionMap mapping;
};

#endif
