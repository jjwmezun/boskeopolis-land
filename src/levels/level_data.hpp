#pragma once

#include <string>

struct LevelData final
{
    bool has_secret_goal_;
    bool has_card_;
    bool has_suits_;
    bool has_crown_;
    unsigned int id_;
    unsigned int cycle_;
    unsigned int theme_;
    unsigned int gem_challenge_;
    unsigned int time_challenge_;
    unsigned int next_level_;
    std::string code_name_;
};