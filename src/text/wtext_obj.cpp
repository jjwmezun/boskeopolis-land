#include "render.hpp"
#include <unordered_map>
#include "wtext_obj.hpp"

static constexpr int CHAR_SIZE_PIXELS = 8;

static CharFrame getFrameIndices( char32_t character )
{
    switch ( character )
    {
        case( ' ' ):
        {
            return { 31, 3 };
        }
        break;

        case( 'Q' ):
        {
            return { 26, 0 };
        }
        break;

        case( 'i' ):
        {
            return { 18, 0 };
        }
        break;

        case( '…' ):
        {
            return { 18, 0 };
        }
        break;

        case( '¡' ):
        {
            return { 6, 1 };
        }
        break;

        case( '₧' ):
        {
            return { 4, 2 };
        }
        break;

        case( '𓐰' ):
        {
            return { 5, 2 };
        }
        break;

        default:
        {
            return { 0, 0 };
        }
        break;
    }
};

static CharFrame getFrame( char32_t character )
{
    CharFrame frame = getFrameIndices( character );
    return { frame.x * CHAR_SIZE_PIXELS, frame.y * CHAR_SIZE_PIXELS };
}

WTextObj::WTextObj()
{

};

WTextObj::WTextObj
(
    const char32_t* text,
    int x,
    int y
)
:
    x_ ( x ),
    y_ ( y )
{
    while ( text[ 0 ] != '\0' )
    {
        frames_.push_back( getFrame( text[ 0 ] ) );
        ++text;
    }
};

void WTextObj::render()
{
    int x = x_;
    for ( const CharFrame& frame : frames_ )
    {
        renderChar( frame, x, y_ );
        x += CHAR_SIZE_PIXELS;
    }
};

void WTextObj::renderChar( const CharFrame character, int x, int y )
{
    Render::renderObject
    (
        "charset.png",
        {
            character.x,
            character.y,
            CHAR_SIZE_PIXELS,
            CHAR_SIZE_PIXELS
        },
        {
            x,
            y,
            CHAR_SIZE_PIXELS,
            CHAR_SIZE_PIXELS
        }
    );
}