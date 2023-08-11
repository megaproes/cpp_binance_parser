// BinanceClient.h
#pragma once
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class BinanceClient
{
public:
	BinanceClient(const std::string &apiKey, const std::string &secretKey);
	std::string getTrades(const std::string &symbol, std::string &startTime, std::string &endTime);
	std::string getIncomeHistory(const std::string &startTime, std::string &endTime, const std::string &limit);
	std::pair<std::string, std::string> parseTimePeriod(const std::string &period);
	std::string get_listen_key();
	std::string url_encode(const std::string &value);

private:
	std::string apiKey;
	std::string secretKey;

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
	std::string hmac_sha256(const std::string &key, const std::string &data);
	std::string getCurrentTimestamp();
	std::string performRequest(const std::string &url, const std::string &params, bool post);
};