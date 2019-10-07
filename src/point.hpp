#pragma once

#include <iostream>
#include <SDL2/SDL.h>

typedef SDL_Point Point;

inline std::ostream &operator<<( std::ostream &os, Point p )
{
    return os << "{ " << p.x << ", " << p.y << " }";
}