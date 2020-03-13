#pragma once

#include "counter_t.hpp"
#include "level.hpp"
#include "unit.hpp"

struct SaveData
{
    bool oxygen_upgrade_;
    CounterT<Unit::TOTAL_FUNDS_MAX, Unit::TOTAL_FUNDS_MIN> total_funds_;
    int current_level_;
	bool health_upgrades_[ Unit::MAX_HEART_UPGRADES ];
    bool special_levels_[ Level::NUMBER_OF_CYCLES ];
    bool been_to_level_[ Level::NUMBER_OF_LEVELS ];
    bool victories_[ Level::NUMBER_OF_LEVELS ];
    bool secret_goals_[ Level::NUMBER_OF_LEVELS ];
    bool diamonds_[ Level::NUMBER_OF_LEVELS ];
    bool crowns_[ Level::NUMBER_OF_LEVELS ];
    bool suits_[ Level::NUMBER_OF_LEVELS ];
    int gem_scores_[ Level::NUMBER_OF_LEVELS ];
    int time_scores_[ Level::NUMBER_OF_LEVELS ];
};