// Parser.h
#pragma once
#include <string>
#include <vector>
#include "Position.h"
#include <chrono>

class Parser
{
public:

	virtual void displayData(const std::vector<Position> &positions) = 0;
	virtual std::string generateFilename() const = 0;

protected:
	std::tm parse_unix_time(const long long time);
};