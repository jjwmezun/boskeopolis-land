#pragma once

#include "counter_t.hpp"
#include "level_list.hpp"
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
    bool been_to_level_[ LevelList::MAX_LEVELS ];
    bool levels_unlocked_[ LevelList::MAX_LEVELS ];
    bool victories_[ LevelList::MAX_LEVELS ];
    bool secret_goals_[ LevelList::MAX_LEVELS ];
    bool diamonds_[ LevelList::MAX_LEVELS ];
    bool crowns_[ LevelList::MAX_LEVELS ];
    bool suits_[ LevelList::MAX_LEVELS ];
    int gem_scores_[ LevelList::MAX_LEVELS ];
    int time_scores_[ LevelList::MAX_LEVELS ];
};