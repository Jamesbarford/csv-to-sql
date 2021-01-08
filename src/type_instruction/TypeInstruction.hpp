#ifndef TYPE_INSTRUCTION_H
#define TYPE_INSTRUCTION_H

#include <string>
#include <map>
#include "../external/datum_parser.hpp"
#include "../type_mapping/TypeMapping.hpp"

typedef std::tuple<std::string, std::string, unsigned int, datum::DataType, datum::Pattern> TypeInstruction;
typedef std::map<std::string, TypeInstruction> TypeInstructionMap;

class TypeMap
{
public:
	TypeInstruction &operator[](std::string const &key);
	TypeMapping to_type_mapping();

private:
	TypeInstructionMap type_map;
};

#endif
