#include "../../include/Parser.h"

std::tm Parser::parse_unix_time(const long long time)
{
	std::chrono::milliseconds ms{time};
	time_t t = std::chrono::duration_cast<std::chrono::seconds>(ms).count();
	return *std::localtime(&t);
}