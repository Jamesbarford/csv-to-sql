# CSV to SQL

## Setup
- run `install_dependencies.sh`
- compile: `make clean` && `make init`

## How to use
It's a bit rough and ready, in `main` the following methods will write to a file:

```cpp
void csv_to_sql_file(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map);
void csv_to_sql_file(std::string const &path, std::string const &table_name);
```

Or if you want the sql as a string:

```cpp
std::string csv_to_sql_string(std::string const &path, std::string const &table_name, TypeInstructionMap &type_instruction_map);
std::string csv_to_sql_string(std::string const &path, std::string const &tab);
```

A `TypeInstructionMap` can be instantiated in the following way:

- column_name -> string
- sql_output_type -> string
- column_index -> unsigned int
- parsing_type -> datum::DataType, of which can be: `datum::DataType::Integer`, `datum::DataType::Date`, `datum::DataType::String`, `datum::DataType::Float`, `datum::DataType::Currency` or `datum::DataType::Percentage`
- pattern -> this is only used for dates e.g to parse: `"2020-04-11"` the pattern would need to be `"%Y-%m-%d"`, a full list can be found: `https://en.cppreference.com/w/cpp/chrono/parse`

If no `TypeInstructionMap` is provided a best guess will be made at what the data types are and the `SQL` output type will be a `SQLite` type. This is highly discouraged as it is an approximation the tendency for inaccuracy would be high.

## Example

csv file which located at `/tmp/example.csv`

```csv
row_number,uuid,start_date,end_date,uuid_2
1,83e0721f-f426-46f8-b2fa-6b3ed6246860,2020-12-23,2020-12-26,2687ec46-0e3d-4842-b582-091050c31252
2,48540f8a-e7a9-4670-8283-6452895f9933,2021-01-11,2021-01-04,3c6627a6-1bae-4865-88b5-95df12cf340b
3,653833a1-4d4b-4b6c-8c6f-08c00e82c6cb,2021-01-05,2020-12-29,242b03bf-887f-4486-8ee6-c412d9da1156
4,5fe8ca99-4b8f-4041-aa30-77305f7d2282,2020-12-24,2021-01-09,18e32fee-446c-4638-a069-ee50da53cc9a
5,b95da496-e7e0-4b10-94a9-ec668a7dc276,2021-01-09,2021-01-15,0a13303a-0a17-490d-9164-82d555bdf226
6,3563b258-dfce-415e-8efc-3836fd8eb409,2021-01-02,2021-01-05,0a012810-4c75-4476-bd1d-25424972b6b9
7,335bc8ad-5fe6-47fd-88b7-4678a57f19fc,2021-01-13,2020-12-24,9c253c49-7ae1-4d3f-9fd3-adb3aa73664f
8,dba7c582-be87-4ecb-9138-2dc30ce7870c,2020-12-25,2021-01-05,0d7f1915-f8af-40d6-9fd0-fe94d85d8ae8
9,4f94dfd7-05b9-44db-a34d-dd97d7705865,2021-01-09,2020-12-23,fc1a3c4d-aff8-48ae-9ac5-b68f83b6e73d
10,0d3350c5-5482-4afd-a90b-b5360a509f0b,2021-01-02,2020-12-25,362df573-c1c1-42f5-ac62-68558749b355
```

Inside `main.cpp`

```cpp
int main(void) {

	TypeInstructionMap type_instruction_map;

	type_instruction_map.insert(TypeInstruction("row_number", "INTEGER", 0, datum::DataType::Integer));
	type_instruction_map.insert(TypeInstruction("uuid", "TEXT", 1, datum::DataType::String));
	type_instruction_map.insert(TypeInstruction("start_date", "TIMESTAMP", 2, datum::DataType::Date, "%Y-%m-%d"));
	type_instruction_map.insert(TypeInstruction("end_date", "TIMESTAMP", 3, datum::DataType::Date, "%Y-%m-%d"));
	type_instruction_map.insert(TypeInstruction("uuid_2", "TEXT", 4, datum::DataType::String));

	csv_to_sql_file("/tmp/example.csv", "example", type_instruction_map);
}
```

This would output the file `"example.sql"`:

```sql
-- -------------------------------------------------------------
-- CSV to sql conversion
-- https://github.com/Jamesbarford
--
-- File name: 2thou
-- Created at: 2021-01-11T11:39:52Z
-- -------------------------------------------------------------

CREATE TABLE "2thou" ("row_number" INTEGER, "uuid" TEXT, "start_date" TIMESTAMP, "end_date" TIMESTAMP, "uuid_2" TEXT);

INSERT INTO "2thou" ("row_number", "uuid", "start_date", "end_date", "uuid_2") VALUES
('1', '83e0721f-f426-46f8-b2fa-6b3ed6246860', '2020-12-23T00:00:00Z', '2020-12-26T00:00:00Z', '2687ec46-0e3d-4842-b582-091050c31252'),
('2', '48540f8a-e7a9-4670-8283-6452895f9933', '2021-01-11T00:00:00Z', '2021-01-04T00:00:00Z', '3c6627a6-1bae-4865-88b5-95df12cf340b'),
('3', '653833a1-4d4b-4b6c-8c6f-08c00e82c6cb', '2021-01-05T00:00:00Z', '2020-12-29T00:00:00Z', '242b03bf-887f-4486-8ee6-c412d9da1156'),
('4', '5fe8ca99-4b8f-4041-aa30-77305f7d2282', '2020-12-24T00:00:00Z', '2021-01-09T00:00:00Z', '18e32fee-446c-4638-a069-ee50da53cc9a'),
('5', 'b95da496-e7e0-4b10-94a9-ec668a7dc276', '2021-01-09T00:00:00Z', '2021-01-15T00:00:00Z', '0a13303a-0a17-490d-9164-82d555bdf226'),
('6', '3563b258-dfce-415e-8efc-3836fd8eb409', '2021-01-02T00:00:00Z', '2021-01-05T00:00:00Z', '0a012810-4c75-4476-bd1d-25424972b6b9'),
('7', '335bc8ad-5fe6-47fd-88b7-4678a57f19fc', '2021-01-13T00:00:00Z', '2020-12-24T00:00:00Z', '9c253c49-7ae1-4d3f-9fd3-adb3aa73664f'),
('8', 'dba7c582-be87-4ecb-9138-2dc30ce7870c', '2020-12-25T00:00:00Z', '2021-01-05T00:00:00Z', '0d7f1915-f8af-40d6-9fd0-fe94d85d8ae8'),
('9', '4f94dfd7-05b9-44db-a34d-dd97d7705865', '2021-01-09T00:00:00Z', '2020-12-23T00:00:00Z', 'fc1a3c4d-aff8-48ae-9ac5-b68f83b6e73d'),
('10', '0d3350c5-5482-4afd-a90b-b5360a509f0b', '2021-01-02T00:00:00Z', '2020-12-25T00:00:00Z', '362df573-c1c1-42f5-ac62-68558749b355'),
('11', '909f96cb-bc93-4a01-9353-f41d438da016', '2021-01-03T00:00:00Z', '2021-01-09T00:00:00Z', '8c7c81ae-184c-4a32-b65b-fd8a6302a877');

```
