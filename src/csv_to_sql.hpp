#ifndef CSV_TO_SQL_H
#define CSV_TO_SQL_H

#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include "./type_mapping/TypeMapping.hpp"
#include "./tokenizer/csv_tokenizer.hpp"
#include "./external/datum_parser.hpp"
#include "./formatting/datum_formatter.hpp"

#define stringify(x) ('\'' + x + '\'')

std::string csv_to_sql(std::string const &path, std::string const &table_name, TypeMapping &type_mapping);
std::string csv_to_sql(std::string const &path, std::string const &table_name);
TypeMapping get_type_mapping(std::string const &path);
std::string datum_type_to_sqlite_type(datum::DataType const &type);
std::string make_insert_statement_from_csv(TypeMapping &type_mapping, std::string const &path);
std::string make_create_statement(TypeMapping &type_mapping, std::string const &table_name);
void write_to_file(std::string &query, std::string const &file_name);

#endif
