#include "../../include/Binance/Trade.h"

Trade::Trade(const nlohmann::json &data)
{
}
Trade::Trade() {}
std::vector<Trade> Trade::getTrades(const nlohmann::json &data)
{
	std::vector<Trade> trades;
	for (auto &d : data)
	{
		Trade t{};
		t.symbol = std::string(d["symbol"]);
		t.id = d["id"];
		t.orderId = d["orderId"];
		t.side = std::string(d["side"]);
		t.price = std::stod(d["price"].get<std::string>());
		t.qty = std::stod(d["qty"].get<std::string>());
		t.realizedPnl = std::stod(d["realizedPnl"].get<std::string>());
		t.marginAsset = d["marginAsset"];
		t.quoteQty = std::stod(d["quoteQty"].get<std::string>());
		t.commission = std::stod(d["commission"].get<std::string>());
		t.commissionAsset = d["commissionAsset"];
		t.time = d["time"];
		t.positionSide = d["positionSide"];
		t.maker = d["maker"];
		t.buyer = d["buyer"];
		trades.push_back(t);
	}
	return trades;
}
