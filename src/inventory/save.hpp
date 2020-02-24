#pragma once

#include "level.hpp"
#include <string>

class Save
{
    public:
        static Save loadFromFile( const std::string& path );
        bool hasError() const;

    private:
        bool oxygen_upgrade_;
        int total_funds_;
        int total_funds_shown_;
        int current_level_;
        int number_of_health_upgrades_;
        int order_;
        std::u32string name_;
        std::vector<std::u32string> errors_;
        bool been_to_level_[ Level::NUMBER_OF_LEVELS ];
        bool victories_[ Level::NUMBER_OF_LEVELS ];
        bool secret_goals_[ Level::NUMBER_OF_LEVELS ];
        bool diamonds_[ Level::NUMBER_OF_LEVELS ];
        bool crowns_[ Level::NUMBER_OF_LEVELS ];
        bool suits_[ Level::NUMBER_OF_LEVELS ];
        int gem_scores_[ Level::NUMBER_OF_LEVELS ];
        int time_scores_[ Level::NUMBER_OF_LEVELS ];
};