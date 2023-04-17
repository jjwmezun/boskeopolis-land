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

        constexpr float right() const { return x + w; };
        constexpr float bottom() const { return y + h; };
        constexpr float centerX() const { return x + w / 2.0f; };
        constexpr float centerY() const { return y + h / 2.0f; };
    };
}

#endif // RECT_H