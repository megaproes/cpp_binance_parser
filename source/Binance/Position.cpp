#include "../../include/Binance/Position.h"

Position::Position(const Trade &trade) : symbol(trade.symbol), side(trade.side)
{
}
Position::Position() : average_entry_price(0.0), count_entry_prices(0), average_exit_price(0.0),
				   count_exit_prices(0), commission(0.0), realizedPnl_NET(0.0), volume_quantity(0.0),
				   exit_volume_quantity(0.0), time_start(0),
				   time_finished(0), volume_dollar(0.0) {}
std::vector<Position> Position::parseTrades(std::vector<Trade> &trades)
{
	std::vector<Position> positions;
	for (size_t i = 0, j = 0; i < trades.size(); i++)
	{
		Position pos{};
		pos.symbol = trades[i].symbol;
		pos.side = trades[i].side;

		pos.count_entry_prices++;
		pos.time_start = trades[i].time;
		pos.commission += trades[i].commission;

		pos.average_entry_price += trades[i].price * trades[i].qty;
		pos.volume_quantity += trades[i].qty;
		pos.volume_dollar += trades[i].quoteQty;
		for (j = i + 1; j <= trades.size(); j++)
		{
			if (pos.side == trades[j].side)
			{
				pos.average_entry_price += trades[j].price * trades[j].qty;
				pos.volume_quantity += trades[j].qty;
				pos.volume_dollar += trades[j].quoteQty;
				pos.commission += trades[j].commission;

				pos.count_entry_prices++;
			}
			else if (pos.side != trades[j].side)
			{
				pos.average_exit_price += trades[j].price * trades[j].qty;
				pos.exit_volume_quantity += trades[j].qty;

				pos.realizedPnl_NET += trades[j].realizedPnl;
				pos.commission += trades[j].commission;

				pos.count_exit_prices++;

				if (abs(pos.volume_quantity - pos.exit_volume_quantity) < 0.000001)
				{
					pos.average_entry_price /= pos.volume_quantity;
					pos.average_exit_price /= pos.exit_volume_quantity;
					pos.realizedPnl_Gross = pos.realizedPnl_NET - pos.commission;
					pos.time_finished = trades[j].time;

					i = j;

					break; // breakes nested 'for'
				}
			}
		}

		positions.push_back(pos);
	}

	return positions;
}
