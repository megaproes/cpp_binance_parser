#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

class Trade
{
public:
    std::string symbol;
    int id;
    int orderId;
    std::string side;
    double price;
    double qty;
    double realizedPnl;
    std::string marginAsset;
    double quoteQty;
    double commission;
    std::string commissionAsset;
    long long time;
    std::string positionSide;
    bool maker;
    bool buyer;

    Trade();
    Trade(const nlohmann::json &data);

    static std::vector<Trade> getTrades(const nlohmann::json &data);
};
