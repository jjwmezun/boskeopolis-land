#pragma once

#include "mezun_sdl2.hpp"

struct OWObject
{
    enum class Type
    {
        LEVEL
    };

    union Value
    {
        int level;
    };

    Type type;
    Value value;
    sdl2::SDLRect position;

    constexpr static OWObject createLevel( int level )
    {
        return { Type::LEVEL, level };
    };
};