#ifndef DATUM_FORMATTER_H
#define DATUM_FORMATTER_H

#include "../external/datum_parser.hpp"

std::string format_datum(datum::Entry const &e, datum::DataType const &t);
std::string to_date_string(long long ms, std::string pattern);

#endif
