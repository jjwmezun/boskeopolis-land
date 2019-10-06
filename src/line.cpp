#include "line.hpp"

std::array<Line, 4> Line::getLinesFromBox( const sdl2::SDLRect& box )
{
    const Line top = { { box.x, box.y }, { box.right(), box. y } };
    const Line right = { { box.right(), box.y }, { box.right(), box.bottom() } };
    const Line bottom = { { box.x, box.bottom() }, { box.right(), box.bottom() } };
    const Line left = { { box.x, box.y }, { box.x, box.bottom() } };
    return { top, right, bottom, left };
}