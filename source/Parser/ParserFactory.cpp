#include "../../include/ParserFactory.h"
#include "../../include/TextParser.h"
#include "../../include/ExcelParser.h"

std::unique_ptr<Parser> ParserFactory::createParser(const std::string &format)
{
	if (format == "text")
	{
		//return std::make_unique<TextParser>();
	}
	else if (format == "excel")
	{
		return std::make_unique<ExcelParser>();
	}

	return nullptr; // Return nullptr or throw an exception for unsupported formats
}