#include "csv_to_sql.hpp"

/***
 *
 * Defaults to SQLite data types, custom mapping to be supported
 *
 * CREATE TABLE Example ('row_number' INTEGER, 'uuid' TEXT, 'start_date' TEXT, 'end_date' TEXT, 'uuid_2' TEXT);
 *
 * INSERT INTO Example VALUES
 * ('1', '83e0721f-f426-46f8-b2fa-6b3ed6246860', '2020-12-23T00:00:00Z', '2020-12-26T00:00:00Z', '2687ec46-0e3d-4842-b582-091050c31252'),
 * ('2', '48540f8a-e7a9-4670-8283-6452895f9933', '2021-01-11T00:00:00Z', '2021-01-04T00:00:00Z', '3c6627a6-1bae-4865-88b5-95df12cf340b'),
 * ('3', '653833a1-4d4b-4b6c-8c6f-08c00e82c6cb', '2021-01-05T00:00:00Z', '2020-12-29T00:00:00Z', '242b03bf-887f-4486-8ee6-c412d9da1156'),
 * ('4', '5fe8ca99-4b8f-4041-aa30-77305f7d2282', '2020-12-24T00:00:00Z', '2021-01-09T00:00:00Z', '18e32fee-446c-4638-a069-ee50da53cc9a');
 *
*/

std::string datum_type_to_sqlite_type(datum::DataType const &type);
std::string make_insert_from_datum();

std::string csv_to_sql(std::string const &path, std::string const &table_name, TypeMapping &type_mapping)
{
	std::ostringstream query;

	query << make_create_statement(type_mapping, table_name);
	query << make_insert_statement_from_csv(type_mapping, path);

	return query.str();
}

std::string csv_to_sql(std::string const &path, std::string const &table_name)
{
	datum::set_error_severity(datum::ErrorSeverity::Silence);
	TypeMapping type_mapping = get_type_mapping(path);
	return csv_to_sql(path, table_name, type_mapping);
}

TypeMapping get_type_mapping(std::string const &path)
{
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

	return TypeMapping::from_headers_sample(headers_map, sample_rows);
}

std::string datum_type_to_sqlite_type(datum::DataType const &type)
{
	switch (type)
	{
	case datum::DataType::Currency:
	case datum::DataType::Float:
	case datum::DataType::Percentage:
		return "REAL";

	case datum::DataType::Integer:
		return "INTEGER";

	case datum::DataType::Date:
	case datum::DataType::String:
		return "TEXT";

	default:
		break;
	}
}

std::string make_create_statement(TypeMapping &type_mapping, std::string const &table_name)
{
	std::ostringstream create_statement;

	create_statement << "CREATE TABLE " << table_name << " (";
	for (auto [header_idx, header] : type_mapping.headers_map)
	{
		if (header_idx != 0)
			create_statement << ' ';

		create_statement << stringify(header) << ' ' << datum_type_to_sqlite_type(type_mapping[header].type);
		if (header_idx + 1 != type_mapping.headers_map.size())
			create_statement << ',';
	}
	create_statement << "); \n\n";

	return create_statement.str();
}

std::string make_insert_statement_from_csv(TypeMapping &type_mapping, std::string const &path)
{
	std::ostringstream insert_statement;

	traverse_csv(path, [&](std::string csv_string, size_t row_index) {
		if (row_index != 0)
			insert_statement << "(";
		split_row(csv_string, [&](std::string raw_data, size_t column_idx) {
			if (row_index != 0)
			{
				datum::Datum d = datum::parse(raw_data, type_mapping[column_idx]);

				d.visit([&](auto entry, auto type) {
					insert_statement << stringify(format_datum(entry, type));
					if (type_mapping.headers_map.size() - 1 != column_idx)
						insert_statement << ", ";
				});
			}
		});
		if (row_index != 0)
			insert_statement << "),\n";
	});

	std::string query_string = insert_statement.str();
	query_string.at(query_string.size() - 2) = ';';

	return query_string;
}

void write_to_file(std::string &query_string, std::string const &file_name)
{
	std::ofstream file_stream;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());

	file_stream.open(file_name);

	file_stream << "-- -------------------------------------------------------------\n"
				<< "-- CSV to sql conversion \n"
				<< "-- https://github.com/Jamesbarford\n"
				<< "--\n"
				<< "-- File name: " << file_name << '\n'
				<< "-- Created at: " << to_date_string(ms.count(), "%Y-%m-%dT%H:%M:%SZ") << '\n'
				<< "-- -------------------------------------------------------------\n\n";

	file_stream << query_string;
	file_stream.close();
}
