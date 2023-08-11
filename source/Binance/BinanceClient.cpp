// BinanceClient.cpp

#include "BinanceClient.h"
#include <iomanip>
BinanceClient::BinanceClient(const std::string &apiKey, const std::string &secretKey) : apiKey(apiKey), secretKey(secretKey)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

size_t BinanceClient::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
	userp->append((char *)contents, size * nmemb);
	return size * nmemb;
}

std::string BinanceClient::hmac_sha256(const std::string &key, const std::string &data)
{
	unsigned char *digest;
	digest = HMAC(EVP_sha256(), key.c_str(), key.size(), (unsigned char *)data.c_str(), data.size(), NULL, NULL);

	std::ostringstream os;
	for (int i = 0; i < 32; i++)
	{
		os << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)digest[i];
	}
	return os.str();
}

std::string BinanceClient::getCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return std::to_string(millis);
}

std::string BinanceClient::performRequest(const std::string &url, const std::string &params, bool post)
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + apiKey).c_str());

		// Sign the request
		std::string signature = hmac_sha256(secretKey, params);
		std::string totalParams = params + (post && !params.empty() ? "&" : "") + "&signature=" + signature;
		std::cout << "Total params: " << totalParams << std::endl;

		// Добавляем параметры в URL для GET-запроса
		std::string finalUrl = post ? url : url + "?" + totalParams;
		curl_easy_setopt(curl, CURLOPT_URL, finalUrl.c_str());

		if (post)
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, totalParams.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	return readBuffer;
}

std::string BinanceClient::get_listen_key()
{
	std::string timestamp = getCurrentTimestamp();
	std::string params = "timestamp=" + timestamp;
	std::cout << params << std::endl;
	return performRequest("https://fapi.binance.com/fapi/v1/listenKey", params, true);
}
std::string BinanceClient::getTrades(const std::string &symbol, std::string &startTime, std::string &endTime)
{
	std::string timestamp = getCurrentTimestamp();
	// endTime = timestamp;
	std::string params = "symbol=" + symbol + "&startTime=" + startTime + "&endTime=" + endTime + "&timestamp=" + timestamp;
	std::cout << params << std::endl;
	return performRequest("https://fapi.binance.com/fapi/v1/userTrades", params, false);
}

std::string BinanceClient::getIncomeHistory(const std::string &startTime, std::string &endTime, const std::string &limit = "")
{
	std::string timestamp = getCurrentTimestamp();
	// endTime = timestamp;
	std::cout << "Making getIncomeHistory func" << std::endl;
	std::string params = "startTime=" + startTime + "&endTime=" + endTime + "&limit=" + limit + "&timestamp=" + timestamp;
	std::cout << params << std::endl;
	return performRequest("https://fapi.binance.com/fapi/v1/income", params, false);
}

std::pair<std::string, std::string> BinanceClient::parseTimePeriod(const std::string &period)
{
	using namespace std::chrono;

	// Получить текущее время
	system_clock::time_point now = system_clock::now();
	time_t now_c = system_clock::to_time_t(now);
	tm local_tm = *localtime(&now_c);

	system_clock::time_point start_of_period;
	system_clock::time_point end_of_period = now;

	if (period == "today")
	{
		local_tm.tm_hour = 0;
		local_tm.tm_min = 0;
		local_tm.tm_sec = 0;
		start_of_period = system_clock::from_time_t(mktime(&local_tm));
	}
	else if (period == "yesterday")
	{
		local_tm.tm_hour = 0;
		local_tm.tm_min = 0;
		local_tm.tm_sec = 0;
		local_tm.tm_mday--;
		start_of_period = system_clock::from_time_t(mktime(&local_tm));

		local_tm.tm_hour = 23;
		local_tm.tm_min = 59;
		local_tm.tm_sec = 59;
		end_of_period = system_clock::from_time_t(mktime(&local_tm));
	}
	else if (period == "this week")
	{
		local_tm.tm_hour = 0;
		local_tm.tm_min = 0;
		local_tm.tm_sec = 0;
		local_tm.tm_mday -= local_tm.tm_wday ? local_tm.tm_wday - 1 : 6; // если сегодня воскресенье (tm_wday == 0), то откатиться на 6 дней назад
		start_of_period = system_clock::from_time_t(mktime(&local_tm));

		// Найти конец недели (6 дней после начала недели)
		local_tm = *localtime(&now_c); // важно обновить local_tm, так как предыдущая операция могла изменить его значение
		local_tm.tm_hour = 23;
		local_tm.tm_min = 59;
		local_tm.tm_sec = 59;
		local_tm.tm_mday += (6 - (local_tm.tm_wday ? local_tm.tm_wday - 1 : 6)); // добавить разницу до конца недели
		end_of_period = system_clock::from_time_t(mktime(&local_tm));
	}
	else // Предполагаем, что это дата в формате "YYYY-MM-DD"
	{
		std::istringstream ss(period);
		std::tm tm = {};
		ss >> std::get_time(&tm, "%Y-%m-%d");

		if (ss.fail())
		{
			std::cerr << "Failed to parse date: " << period << std::endl;
			exit(1);
		}

		// Считаем, что день начинается в 00:00:00
		tm.tm_hour = 0;
		tm.tm_min = 0;
		tm.tm_sec = 0;
		start_of_period = system_clock::from_time_t(mktime(&tm));

		// Считаем, что день заканчивается в 23:59:59
		tm.tm_hour = 23;
		tm.tm_min = 59;
		tm.tm_sec = 59;
		end_of_period = system_clock::from_time_t(mktime(&tm));
	}
	// Преобразовать время в миллисекунды
	auto start_ms = duration_cast<milliseconds>(start_of_period.time_since_epoch()).count();
	auto end_ms = duration_cast<milliseconds>(end_of_period.time_since_epoch()).count();

	// Возвращаем пару строк
	return {std::to_string(start_ms), std::to_string(end_ms)};
}
