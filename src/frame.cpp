#include "frame.hpp"
#include "render.hpp"

void Frame::render() const
{
    // Render BG
    if ( color_ > -1 )
    {
        Render::renderRect( { coords_.x + 6, coords_.y + 6, coords_.w - 12, coords_.h - 12 }, color_ );
    }

    // Left
    Render::renderObject( "bg/frame.png", { 0, 7, 7, 1 }, { coords_.x, coords_.y + 7, 7, coords_.h - 14 } );

    // Right
    Render::renderObject( "bg/frame.png", { 8, 7, 7, 1 }, { coords_.x + coords_.w - 7, coords_.y + 7, 7, coords_.h - 14 } );

    // Top
    Render::renderObject( "bg/frame.png", { 7, 0, 1, 7 }, { coords_.x + 7, coords_.y, coords_.w - 14, 7 } );

    // Bottom
    Render::renderObject( "bg/frame.png", { 7, 8, 1, 7 }, { coords_.x + 7, coords_.y + coords_.h - 7, coords_.w - 14, 7 } );

    // Top left
    Render::renderObject( "bg/frame.png", { 0, 0, 7, 7 }, { coords_.x, coords_.y, 7, 7 } );

    // Bottom left
    Render::renderObject( "bg/frame.png", { 0, 8, 7, 7 }, { coords_.x, coords_.y + coords_.h - 7, 7, 7 } );

    //Top right
    Render::renderObject( "bg/frame.png", { 8, 0, 7, 7 }, { coords_.x + coords_.w - 7, coords_.y, 7, 7 } );

    //Bottom right
    Render::renderObject( "bg/frame.png", { 8, 8, 7, 7 }, { coords_.x + coords_.w - 7, coords_.y + coords_.h - 7, 7, 7 } );
};