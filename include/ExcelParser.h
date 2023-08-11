#pragma once
#include "Parser.h"

class ExcelParser : public Parser
{
public:
	void displayData(const std::vector<Position> &positions) override;

private:
	std::string generateFilename() const override;
};