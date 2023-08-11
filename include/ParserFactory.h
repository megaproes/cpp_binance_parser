#pragma once
#include <memory>
#include <string>
#include "Parser.h"

class ParserFactory
{
public:
	static std::unique_ptr<Parser> createParser(const std::string &format);
};