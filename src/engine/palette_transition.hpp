#pragma once

#include "palette.hpp"

struct PaletteTransition
{
    bool set;
    int start;
    int max;
    Palette from;
    Palette to;
};