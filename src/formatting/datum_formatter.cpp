#include "datum_formatter.hpp"

std::string to_date_string(long long ms, std::string pattern)
{
	auto date_ms = std::chrono::milliseconds(ms);
	auto time_point = std::chrono::time_point<std::chrono::system_clock>(date_ms);
	auto in_time_t = std::chrono::system_clock::to_time_t(time_point);
	auto local_time = std::localtime(&in_time_t);

	std::stringstream ss;
	ss << std::put_time(local_time, pattern.c_str());
	return ss.str();
}

std::string format_datum(datum::Entry const &e, datum::DataType const &type)
{
	switch (type)
	{
	case datum::DataType::Date:
		return to_date_string(std::get<unsigned long>(e), "%Y-%m-%dT%H:%M:%SZ");

	case datum::DataType::Float:
	case datum::DataType::Percentage:
	case datum::DataType::Currency:
	{
		long double d = std::get<long double>(e);
		std::ostringstream formatted;
		formatted << d;
		return formatted.str();
	}

	case datum::DataType::Integer:
		return std::to_string(std::get<long long>(e));

	case datum::DataType::String:
		return std::get<std::string>(e);

	default:
		break;
	}
}
