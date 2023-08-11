#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include "Trade.h"

class Position
{
public:
    Position();
    Position(const Trade& trade);

    std::string symbol;
    std::string side;

    double average_entry_price;
    int count_entry_prices;

    double average_exit_price;
    int count_exit_prices;

    double commission;
    double realizedPnl_NET;
    double realizedPnl_Gross;

    double volume_dollar;
    double volume_quantity; // qty
    double exit_volume_quantity;

    long long time_start;
    long long time_finished;

    std::vector<Trade> all_trades_inside_position;

    static std::vector<Position> parseTrades(std::vector<Trade>& trades);
};
