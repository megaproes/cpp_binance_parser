#include "../../include/ExcelParser.h"
#include "../../libs/xlnt/xlnt.hpp"
#include <chrono>
#include <filesystem>
void ExcelParser::displayData(const std::vector<Position> &positions)
{
	xlnt::workbook wb;
	auto ws = wb.active_sheet();
	// "%Y-%m-%d %H:%M:%S"
	// Запись заголовков
	ws.cell("A1").value("Data");
	ws.cell("B1").value("Time entry");
	ws.cell("C1").value("Time exit");
	ws.cell("D1").value("Ticker");
	ws.cell("E1").value("L / S");
	ws.cell("F1").value("Average Entry");
	ws.cell("G1").value("Average Exit");
	ws.cell("H1").value("Volume");
	ws.cell("I1").value("$Volume");
	ws.cell("J1").value("Commision");
	ws.cell("K1").value("P / L Gross");
	ws.cell("L1").value("P / L NET");

	// Запись данных о позициях
	for (size_t i = 0; i < positions.size(); ++i)
	{
		auto &pos = positions[i];
		int row = i + 2; // Строки в Excel начинаются с 1, и первая строка уже занята заголовками
		std::tm tm_start = parse_unix_time(pos.time_start);
		std::tm tm_finish = parse_unix_time(pos.time_finished);

		std::stringstream ss_start;
		ss_start << std::put_time(&tm_start, "%H:%M:%S");

		std::stringstream ss_finish;
		ss_finish << std::put_time(&tm_finish, "%H:%M:%S");

		std::stringstream ss_day_start;
		ss_day_start << std::put_time(&tm_start, "%Y-%m-%d");
		ws.cell("A" + std::to_string(row)).number_format(xlnt::number_format::date_xlsx14());
		ws.cell("A" + std::to_string(row)).value(ss_day_start.str());

		ws.cell("B" + std::to_string(row)).value(ss_start.str());
		ws.cell("B" + std::to_string(row)).number_format(xlnt::number_format::date_xlsx14());

		ws.cell("C" + std::to_string(row)).value(ss_finish.str());
		ws.cell("C" + std::to_string(row)).number_format(xlnt::number_format::date_xlsx14());

		ws.cell("D" + std::to_string(row)).value(pos.symbol);
		ws.cell("E" + std::to_string(row)).value(pos.side);
		ws.cell("F" + std::to_string(row)).value(pos.average_entry_price);
		ws.cell("G" + std::to_string(row)).value(pos.average_exit_price);
		ws.cell("H" + std::to_string(row)).value(pos.volume_quantity);
		ws.cell("I" + std::to_string(row)).value(pos.volume_dollar);
		ws.cell("J" + std::to_string(row)).value(pos.commission);
		ws.cell("K" + std::to_string(row)).value(pos.realizedPnl_Gross);
		ws.cell("L" + std::to_string(row)).value(pos.realizedPnl_NET);
	}

	// Сохранение файла
	wb.save(generateFilename());
}

std::string ExcelParser::generateFilename() const
{
	std::filesystem::path output_dir("output");

	// Create the directory if it doesn't exist
	if (!std::filesystem::exists(output_dir))
	{
		std::filesystem::create_directory(output_dir);
	}

	auto now{std::chrono::system_clock::now()};
	std::time_t current_time_t{std::chrono::system_clock::to_time_t(now)};

	// Create a stringstream to build the filename
	std::stringstream ss;
	ss << output_dir.string() << "/" << std::put_time(std::localtime(&current_time_t), "%Y-%m-%d-%H-%M-%S");

	return ss.str() + ".xlsx";
}
