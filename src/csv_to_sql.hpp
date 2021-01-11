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

std::string csv_to_sql(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map);
std::string csv_to_sql(std::string const &path, std::string const &table_name);
std::string make_insert_statement_from_csv(TypeInstructionMap &type_instruction_map, std::string const &path, std::string const &table_name);
std::string make_create_statement(TypeInstructionMap &type_instruction_map, std::string const &table_name);
void write_to_file(std::string &query, std::string const &file_name);

#endif
