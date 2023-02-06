#ifndef RECT_H
#define RECT_H

namespace BSL
{
    struct Rect
    {
        float x;
        float y;
        float w;
        float h;

        Rect( float _x, float _y, float _w, float _h );
        float right() const;
        float bottom() const;
    };
}

#endif // RECT_H