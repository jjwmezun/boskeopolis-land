#pragma once

#include "mezun_math.hpp"
#include "line.hpp"

namespace mezun
{
    static constexpr double EPSILON = 0.000001;

    static constexpr double getCrossProduct( Point a, Point b )
    {
        return a.x * b.y - b.x * a.y;
    };

    static constexpr bool isPointOnLine( const Line& line, Point point )
    {
        const Line a = { { 0, 0 }, { line.p2.x - line.p1.x, line.p2.y - line.p1.y } };
        const Point b = { point.x - line.p1.x, point.y - line.p1.y };
        double c = getCrossProduct( a.p2, b );
        return std::abs( c ) < EPSILON;
    };

    static constexpr bool isPointRightOfLine( const Line& line, Point point )
    {
        const Line a = { { 0, 0 }, { line.p2.x - line.p1.x, line.p2.y - line.p1.y } };
        const Point b = { point.x - line.p1.x, point.y - line.p1.y };
        double c = getCrossProduct( a.p2, b );
        return c < 0;
    };

    static constexpr bool lineCrossesLine( const Line& l1, const Line& l2 )
    {
        return isPointOnLine( l1, l2.p1 )
            || isPointOnLine( l1, l2.p2 )
            || (
                    isPointRightOfLine( l1, l2.p1 )
                    ^ isPointRightOfLine( l1, l2.p2 )
                );
    };

    static constexpr bool testLineAndBoxCollision( const Line& line, const sdl2::SDLRect& box )
    {
        const sdl2::SDLRect line_box = line.getBox();
        if ( box.testSimpleCollision( line_box ) )
        {
            const auto box_lines = Line::getLinesFromBox( box );
            for ( const Line& box_line : box_lines )
            {
                if ( lineCrossesLine( line, box_line ) && lineCrossesLine( box_line, line ) )
                {
                    return true;
                }
            }
        }
        return false;
    };
}