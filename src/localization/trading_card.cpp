#include "trading_card.hpp"

TradingCard::TradingCard( std::u32string name, std::u32string description, std::u32string episode )
:
    name_ ( name ),
    description_ ( description ),
    episode_ ( episode )
{};