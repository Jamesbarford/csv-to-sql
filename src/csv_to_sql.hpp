#ifndef CSV_TO_SQL_H
#define CSV_TO_SQL_H

#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include "./tokenizer/csv_tokenizer.hpp"
#include "./external/datum_parser.hpp"
#include "./formatting/datum_formatter.hpp"
#include "./type_instruction/TypeInstruction.hpp"

#define stringify(x) ('\'' + x + '\'')
#define stringify_double(x) ('"' + x + '"')

std::string csv_to_sql_string(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map);
std::string csv_to_sql_string(std::string const &path, std::string const &table_name);

void csv_to_sql_file(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map);
void csv_to_sql_file(std::string const &path, std::string const &table_name);

#endif
