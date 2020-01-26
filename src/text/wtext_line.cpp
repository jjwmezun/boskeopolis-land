#include <algorithm>
#include <cassert>
#include "localization.hpp"
#include "localization_language.hpp"
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
    for ( const WTextCharacter& frame : frames_ )
    {
        renderChar( frame, x, y_ );
        x += WTextCharacter::SIZE_PIXELS;
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
        x += WTextCharacter::SIZE_PIXELS;
    }
}


void WTextLine::render( int limit ) const
{
    const bool shadow_set = shadow_frames_.size() == frames_.size();
    if ( shadow_set )
    {
        renderWithShadow( limit );
    }
    else
    {
        renderWithoutShadow( limit );
    }
}

void WTextLine::renderWithoutShadow( int limit ) const
{
    int x = x_;
    limit = ( limit < 0 ) ? frames_.size() : std::min( limit, ( int )( frames_.size() ) );
    for ( int i = 0; i < limit; ++i )
    {
        renderChar( frames_[ i ], x, y_ );
        x += WTextCharacter::SIZE_PIXELS;
    }
}

void WTextLine::renderWithShadow( int limit ) const
{
    assert( frames_.size() == shadow_frames_.size() );
    int x = x_;
    limit = ( limit < 0 ) ? frames_.size() : std::min( limit, ( int )( frames_.size() ) );
    for ( int i = 0; i < limit; ++i )
    {
        renderChar( shadow_frames_[ i ], x + 1, y_ + 1 );
        renderChar( frames_[ i ], x, y_ );
        x += WTextCharacter::SIZE_PIXELS;
    }
}

void WTextLine::renderChar( const WTextCharacter character, int x, int y ) const
{
    Render::renderObject
    (
        Localization::getCurrentLanguage().getCharsetImageSrc(),
        {
            character.x(),
            character.y(),
            WTextCharacter::SIZE_PIXELS,
            WTextCharacter::SIZE_PIXELS
        },
        {
            x,
            y,
            WTextCharacter::SIZE_PIXELS,
            WTextCharacter::SIZE_PIXELS
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