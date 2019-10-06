#pragma once

#include <iostream>

struct Point
{
    int x;
    int y;
};

inline std::ostream &operator<<( std::ostream &os, Point p )
{
    return os << "{ " << p.x << ", " << p.y << " }";
}