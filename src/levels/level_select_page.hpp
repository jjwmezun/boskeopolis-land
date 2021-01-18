#pragma once

#include "level_data.hpp"

struct LevelSelectPage
{
    static constexpr int MAX_LEVELS = 8;

    unsigned int cycle_;
    unsigned int number_of_levels_;
    LevelData levels_[ MAX_LEVELS ];
};