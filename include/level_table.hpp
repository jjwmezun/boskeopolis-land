#ifndef LEVEL_TABLE_H
#define LEVEL_TABLE_H

#include "goal.hpp"
#include <string>
#include <vector>

namespace BSL
{
    struct LevelData
    {
        std::string name;
        uint_fast16_t gemscore;
        uint_fast16_t timescore;
        uint_fast16_t entrancex;
        uint_fast16_t entrancey;
        uint_fast16_t camerax;
        uint_fast16_t cameray;
        std::vector<std::string> maps;
        uint_fast8_t mapcount;
        Goal goal;
    };

    extern std::vector<LevelData> level_table;

    void loadLevelDataTable();
};

#endif // LEVEL_TABLE_H