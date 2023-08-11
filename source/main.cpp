#include <iostream>
#include "Binance/BinanceClient.h"
#include "../libs/nlohmann/json.hpp"
#include <set>
#include "Position.h"
#include "Trade.h"
#include <memory>
#include <Parser.h>
#include "ParserFactory.h"
#include <fstream>
int main()
{
	// std::string apiKey = "1Su30ANOkWEBHgJbJ4SYWOFMg2IFBXVbbyA0mojqPfHGoL65egDc64Fg9oIQU73j";
	// std::string secretKey = "ulMm5wpCmgvBEAlVRNvQbbMvIbcOIv4MYwag8Ba0ZhTSJJpFxczrfhYqLlDn04Md";
	std::string apiKey;
	std::string secretKey;

	// Open the configuration file
	std::ifstream configFile("config/config.json");

	if (!configFile)
	{
		std::cerr << "Error: Unable to open configuration file.\n";
		return 1;
	}

	// Parse the JSON
	nlohmann::json config;
	try
	{
		configFile >> config;
	}
	catch (nlohmann::json::parse_error &e)
	{
		std::cerr << "Error: Invalid JSON structure in configuration file.\n";
		return 1;
	}

	// Get the keys
	if (config.contains("apiKey") && config.contains("secretKey"))
	{
		apiKey = config["apiKey"];
		secretKey = config["secretKey"];
	}
	else
	{
		std::cerr << "Error: API keys not found in configuration file.\n";
		return 1;
	}

	BinanceClient client(apiKey, secretKey);
	std::cout << client.get_listen_key() << std::endl;
	//return 1;
	std::string start_time;
	std::string end_time;

	std::cout << "Enter time period (today | yesterday | this week | YYYY-MM--DD): ";
	std::cout << "\n";
	std::string time_period;
	std::getline(std::cin, time_period);

	auto timePeriod = client.parseTimePeriod(time_period);
	start_time = timePeriod.first;
	end_time = timePeriod.second;

	auto data = client.getIncomeHistory(start_time, end_time, "1000");
	std::cout << data << std::endl;

	nlohmann::json data_trades = nlohmann::json::parse(data);
	std::set<std::string> symbols;

	for (const auto &item : data_trades)
	{
		// std::cout << std::string(item["symbol"]) << std::endl; // Log the item

		if (item.contains("symbol"))
		{
			std::string symbol = item["symbol"];
			symbols.insert(symbol);
		}
		else
		{
			std::cout << "Item does not contain a string key 'symbol': " << item << std::endl;
		}
	}

	// just for logs
	for (const auto &symbol : symbols)
		std::cout << symbol << "\t";

	std::cout << std::endl;
	std::cout << start_time << "\t\t" << end_time << std::endl;

	// Выполнение запросов для каждого тикера
	std::vector<Trade> all_trades;
	for (const auto &ticker : symbols)
	{
		std::string clientTrades{client.getTrades(ticker, start_time, end_time)};
		// std::cout << clientTrades << std::endl;
		nlohmann::json data = nlohmann::json::parse(clientTrades);
		std::vector<Trade> trades = Trade::getTrades(data);
		all_trades.insert(all_trades.end(), trades.begin(), trades.end());
	}

	// Парсинг всех сделок в позиции
	std::vector<Position> positions = Position::parseTrades(all_trades);

	// Сортировка позиций по времени начала
	std::sort(positions.begin(), positions.end(), [](const Position &a, const Position &b)
			{ return a.time_start < b.time_start; });

	std::unique_ptr<Parser> parser = ParserFactory::createParser("excel");
	parser->displayData(positions);
	
	return 0;
}