#pragma once

#include <string>

class TradingCard final
{
    public:
        TradingCard( std::u32string name, std::u32string description );
        std::u32string name_;
        std::u32string description_;
        char scratch_[300];
};