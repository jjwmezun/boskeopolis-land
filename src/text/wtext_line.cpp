#include <cassert>
#include "render.hpp"
#include "wtext_line.hpp"

void WTextLine::render() const
{
    const bool shadow_set = shadow_frames_.size() == frames_.size();
    if ( shadow_set )
    {
        renderWithShadow();
    }
    else
    {
        renderWithoutShadow();
    }
}

void WTextLine::renderWithoutShadow() const
{
    int x = x_;
    for ( const CharFrame& frame : frames_ )
    {
        renderChar( frame, x, y_ );
        x += CharFrame::SIZE_PIXELS;
    }
}

void WTextLine::renderWithShadow() const
{
    assert( frames_.size() == shadow_frames_.size() );
    int x = x_;
    for ( int i = 0; i < frames_.size(); ++i )
    {
        renderChar( shadow_frames_[ i ], x + 1, y_ + 1 );
        renderChar( frames_[ i ], x, y_ );
        x += CharFrame::SIZE_PIXELS;
    }
}

void WTextLine::renderChar( const CharFrame character, int x, int y ) const
{
    Render::renderObject
    (
        "charset.png",
        {
            character.x(),
            character.y(),
            CharFrame::SIZE_PIXELS,
            CharFrame::SIZE_PIXELS
        },
        {
            x,
            y,
            CharFrame::SIZE_PIXELS,
            CharFrame::SIZE_PIXELS
        }
    );
}

void WTextLine::changeColorOffset( int offset )
{
    for ( auto& frame : frames_ )
    {
        frame.changeColorOffset( offset );
    }
};