#include "rect.hpp"

namespace BSL
{
    Rect::Rect( float _x, float _y, float _w, float _h )
    :
        x ( _x ),
        y ( _y ),
        w ( _w ),
        h ( _h )
    {};

    float Rect::right() const
    {
        return x + w;
    }

    float Rect::bottom() const
    {
        return y + h;
    }
}