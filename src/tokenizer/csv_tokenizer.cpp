#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

#include "csv_tokenizer.hpp"

void traverse_csv(std::string const &path, unsigned int const &limit, std::function<void(std::string csv_string, size_t row)> iteratee)
{
	size_t row = 0;
	std::ifstream infile(path);
	std::string csv_string;

	while (std::getline(infile, csv_string) && row++ < limit)
		iteratee(csv_string, row);

	infile.close();
}

void traverse_csv(std::string const &path, std::function<void(std::string csv_string, size_t row)> iteratee)
{
	size_t row = 0;
	std::ifstream infile(path);
	std::string csv_string;

	while (std::getline(infile, csv_string))
		iteratee(csv_string, row++);

	infile.close();
}

void split_row(std::string csv_string, std::function<void(std::string row_item, size_t column_idx)> iteratee)
{
	size_t pos = 0;
	size_t column_idx = 0;

	while ((pos = csv_string.find(COMMA)) != std::string::npos)
	{
		iteratee(csv_string.substr(0, pos), column_idx++);
		csv_string.erase(0, pos + 1);
	}

	iteratee(csv_string, column_idx);
}

// grim, but fast a bit faster due to buffer size
void csv_reader(std::string const &path, std::function<void(size_t row_idx, size_t column_idx, std::string value)> iteratee)
{
	int fd = open(path.c_str(), O_RDONLY, S_IWUSR | S_IRUSR);
	if (fd == NULL || fd == EOF)
		std::cout << "error reading: " << path << '\n';
	else
	{
		char buf[BUFFER_SIZE];
		char next_char;
		size_t bytes;
		size_t row_idx = 0;
		size_t char_idx = 0;
		size_t column_idx = 0;
		std::string current_str;

		while ((bytes = read(fd, buf, BUFFER_SIZE)) > 0)
		{
			while ((char_idx < BUFFER_SIZE || char_idx < bytes))
			{
				next_char = buf[char_idx++];
				if (next_char != COMMA && next_char != NEW_LINE)
					current_str += next_char;
				if ((next_char == COMMA) || (next_char == NEW_LINE))
				{
					iteratee(row_idx, column_idx++, current_str);
					current_str.clear();
				}
				if (next_char == NEW_LINE)
				{
					column_idx = 0;
					row_idx++;
				}
			}
			char_idx = 0;
		}
		iteratee(row_idx, column_idx++, current_str);
	}

	close(fd);
}
