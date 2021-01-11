#include "csv_to_sql.hpp"

/***
 *
 * Defaults to SQLite data types, if no mapping specified
 *
 * CREATE TABLE Example ('row_number' INTEGER, 'uuid' TEXT, 'start_date' TEXT, 'end_date' TEXT, 'uuid_2' TEXT);
 *
 * INSERT INTO "Example" ("row_number" , "uuid" , "start_date" , "end_date" , "uuid_2" ) VALUES
 * ('1', '83e0721f-f426-46f8-b2fa-6b3ed6246860', '2020-12-23T00:00:00Z', '2020-12-26T00:00:00Z', '2687ec46-0e3d-4842-b582-091050c31252'),
 * ('2', '48540f8a-e7a9-4670-8283-6452895f9933', '2021-01-11T00:00:00Z', '2021-01-04T00:00:00Z', '3c6627a6-1bae-4865-88b5-95df12cf340b'),
 * ('3', '653833a1-4d4b-4b6c-8c6f-08c00e82c6cb', '2021-01-05T00:00:00Z', '2020-12-29T00:00:00Z', '242b03bf-887f-4486-8ee6-c412d9da1156'),
 * ('4', '5fe8ca99-4b8f-4041-aa30-77305f7d2282', '2020-12-24T00:00:00Z', '2021-01-09T00:00:00Z', '18e32fee-446c-4638-a069-ee50da53cc9a');
 *
*/

TypeInstructionMap create_type_instruction_map_from_sample(std::string const &path);
void write_sql_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name, std::string const &path);
void write_header(std::ostream &query, std::string const &file_name);
void write_create_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name);
void write_insert_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name, std::string const &path);

std::string csv_to_sql_string(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map)
{
	datum::set_error_severity(datum::ErrorSeverity::Logging);
	std::ostringstream query;
	write_sql_statement(query, type_instruction_map, table_name, path);
	return query.str();
}

void csv_to_sql_file(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map)
{
	datum::set_error_severity(datum::ErrorSeverity::Logging);
	std::ofstream query;
	query.open(table_name + ".sql");
	write_sql_statement(query, type_instruction_map, table_name, path);
	query.close();
}

void write_sql_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name, std::string const &path)
{
	write_header(query, table_name);
	write_create_statement(query, type_instruction_map, table_name);
	write_insert_statement(query, type_instruction_map, table_name, path);
}

void write_header(std::ostream &query, std::string const &file_name)
{
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());

	query << "-- -------------------------------------------------------------\n"
		  << "-- CSV to sql conversion \n"
		  << "-- https://github.com/Jamesbarford\n"
		  << "--\n"
		  << "-- File name: " << file_name << '\n'
		  << "-- Created at: " << to_date_string(ms.count(), "%Y-%m-%dT%H:%M:%SZ") << '\n'
		  << "-- -------------------------------------------------------------\n\n";
}

void write_create_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name)
{
	query << "CREATE TABLE " << stringify_double(table_name) << " (";

	type_instruction_map.for_each([&](std::string const &header, TypeInstruction const &type_instruction) -> void {
		std::string output_type = std::get<1>(type_instruction);
		unsigned int column_index = std::get<2>(type_instruction);

		query << stringify_double(header) << ' ' << output_type;

		if (column_index + 1 != type_instruction_map.size())
			query << ", ";
	});

	query << ");\n\n";
}

void write_insert_statement(std::ostream &query, TypeInstructionMap &type_instruction_map, std::string const &table_name, std::string const &path)
{
	query << "INSERT INTO " << stringify_double(table_name) << " (";

	type_instruction_map.for_each([&](std::string const &header, TypeInstruction const &type_instruction) -> void {
		unsigned int column_index = std::get<2>(type_instruction);

		query << stringify_double(header);
		if (column_index + 1 != type_instruction_map.size())
			query << ", ";
	});

	query << ") VALUES\n";

	traverse_csv(path, [&](std::string csv_string, size_t row_index) {
		if (row_index != 0)
			query << "(";
		split_row(csv_string, [&](std::string raw_data, size_t column_idx) {
			if (row_index != 0)
			{
				TypeInstruction type_instruction = type_instruction_map.at(column_idx);
				datum::DataType datum_type = std::get<3>(type_instruction);
				datum::Pattern datum_pattern = std::get<4>(type_instruction);

				datum::Datum d = datum::parse(raw_data, datum::ParseInstruction(datum_type, datum_pattern));

				d.visit([&](auto entry, auto type) {
					query << stringify(format_datum(entry, type));
					if (type_instruction_map.size() - 1 != column_idx)
						query << ", ";
				});
			}
		});
		if (row_index != 0)
			query << "),\n";
	});

	// rewind by two and replace the ",\n" with a ";\n" to terminate sql
	query.seekp(-2, std::ios_base::end);
	query << ";\n";
}

/**
 * These methods take a sample of the data to make an educated approximation of conversions
 * and output types by attempting to parse the first 100 rows
 *
*/

void csv_to_sql_file(std::string const &path, std::string const &table_name)
{
	TypeInstructionMap type_instruction_map = create_type_instruction_map_from_sample(path);
	csv_to_sql_file(path, table_name, type_instruction_map);
}

std::string csv_to_sql_string(std::string const &path, std::string const &table_name)
{
	TypeInstructionMap type_instruction_map = create_type_instruction_map_from_sample(path);
	return csv_to_sql_string(path, table_name, type_instruction_map);
}

TypeInstructionMap create_type_instruction_map_from_sample(std::string const &path)
{
	datum::set_error_severity(datum::ErrorSeverity::Silence);
	ColumnToHeader headers_map;
	SampleRows sample_rows;
	std::vector<std::string> partial_rows;

	traverse_csv(path, 100, [&](std::string csv_string, size_t row_index) {
		split_row(csv_string, [&](std::string raw_data, size_t column_idx) {
			if (row_index == 1)
				headers_map.insert({column_idx, raw_data});
			else
			{
				partial_rows.push_back(raw_data);
				if (column_idx == headers_map.size() - 1)
				{
					sample_rows.push_back(partial_rows);
					partial_rows.clear();
				}
			}
		});
	});

	return TypeInstructionMap::__from_headers_sample(headers_map, sample_rows);
}
