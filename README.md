# CSV to SQL

## Setup
- run `install_dependencies.sh`
- compile: `make clean` && `make init`

## How to use
Requires 3 arguments:
1. path to csv i.e ./test.csv
2. name of database table to create i.e test
3. json schema

```
./csv-sql ./example.csv example ./example.json
```

```csv
row_number,uuid,start_date,end_date,uuid_2
1,83e0721f-f426-46f8-b2fa-6b3ed6246860,2020-12-23,2020-12-26,2687ec46-0e3d-4842-b582-091050c31252
2,48540f8a-e7a9-4670-8283-6452895f9933,2021-01-11,2021-01-04,3c6627a6-1bae-4865-88b5-95df12cf340b
3,653833a1-4d4b-4b6c-8c6f-08c00e82c6cb,2021-01-05,2020-12-29,242b03bf-887f-4486-8ee6-c412d9da1156
```

This would output the file `"example.sql"`:

```sql
CREATE TABLE "example" ("row_number" INTEGER, "uuid" TEXT, "start_date" TIMESTAMP, "end_date" TIMESTAMP, "uuid_2" TEXT);

INSERT INTO "example" ("row_number", "uuid", "start_date", "end_date", "uuid_2") VALUES
('1', '83e0721f-f426-46f8-b2fa-6b3ed6246860', '2020-12-23T00:00:00Z', '2020-12-26T00:00:00Z', '2687ec46-0e3d-4842-b582-091050c31252'),
('2', '48540f8a-e7a9-4670-8283-6452895f9933', '2021-01-11T00:00:00Z', '2021-01-04T00:00:00Z', '3c6627a6-1bae-4865-88b5-95df12cf340b'),
('3', '653833a1-4d4b-4b6c-8c6f-08c00e82c6cb', '2021-01-05T00:00:00Z', '2020-12-29T00:00:00Z', '242b03bf-887f-4486-8ee6-c412d9da1156');
```

## JSON schema
The json provides mapping to the csv. The key is the name of the csv column.

The value is an array:
- `0:` `SQL` type i.e `"INTEGER"`
- `1:` column index, which column this refers to
- `2:` raw data type
- `3:` date pattern

__example:__

```json
{
	"row_number": ["INTEGER", 0, "int"],
	"uuid": ["VARCHAR(50)", 1, "string"],
	"start_date": ["TIMESTAMP", 2, "date", "%Y/%m/%d"],
	"end_date": ["TIMESTAMP", 3, "date", "%Y/%m/%d"],
	"uuid_2": ["TEXT", 4, "string"],
}
```

### raw data types

```
"string"
"date"
"float"
"int"
"currency"
"percentage"
```
