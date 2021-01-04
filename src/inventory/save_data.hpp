#pragma once

#include "counter_t.hpp"
#include "level.hpp"
#include "ow_tile.hpp"
#include "unit.hpp"

struct SaveData
{
    bool oxygen_upgrade_;
    bool extra_palettes_;
    bool gray_only_;
    bool green_only_;
    CounterT<Unit::TOTAL_FUNDS_MAX, Unit::TOTAL_FUNDS_MIN> total_funds_;
    OWTile current_space_;
	bool health_upgrades_[ Unit::MAX_HEART_UPGRADES ];
    bool been_to_level_[ Level::NUMBER_OF_LEVELS ];
    bool levels_unlocked_[ Level::NUMBER_OF_LEVELS ];
    bool victories_[ Level::NUMBER_OF_LEVELS ];
    bool secret_goals_[ Level::NUMBER_OF_LEVELS ];
    bool diamonds_[ Level::NUMBER_OF_LEVELS ];
    bool crowns_[ Level::NUMBER_OF_LEVELS ];
    bool suits_[ Level::NUMBER_OF_LEVELS ];
    int gem_scores_[ Level::NUMBER_OF_LEVELS ];
    int time_scores_[ Level::NUMBER_OF_LEVELS ];
};