#ifndef TYPE_INSTRUCTION_H
#define TYPE_INSTRUCTION_H

#include <string>
#include <map>
#include <functional>
#include "../external/datum_parser.hpp"
#include "../type_heuristics/TypeHeuristicMap.hpp"

// TypeInstruction("start_date", "TIMESTAMP", 0, datum::DataType::Date, "%Y-%m-%d")
typedef std::tuple<std::string, std::string, unsigned int, datum::DataType, datum::Pattern> TypeInstruction;

datum::DataType raw_to_datum_type(std::string const &raw_type);

class TypeInstructionMap
{
public:
	void for_each(std::function<void(std::string const &, TypeInstruction const &)>);
	void insert(TypeInstruction const &type_instruction);
	TypeInstruction at(unsigned int const &column_index);
	TypeInstructionMap static __from_headers_sample(ColumnToHeader const &headers_map, SampleRows const &sample_rows);
	unsigned int size();

private:
	std::map<std::string, TypeInstruction> type_map;
	ColumnToHeader column_to_header;
};

#endif
