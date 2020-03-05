#pragma once

#include "point.hpp"
#include "mezun_sdl2.hpp"
#include <array>

struct Line
{
    Point p1;
    Point p2;
    inline constexpr sdl2::SDLRect getBox() const
    {
        return
        {
            std::min( p1.x, p2.x ),
            std::min( p1.y, p2.y ),
            std::abs( p1.x - p2.x ),
            std::abs( p1.y - p2.y )
        };
    };
    static std::array<Line, 4> getLinesFromBox( const sdl2::SDLRect& box );
};