#include "wtext_character.hpp"
#include "localization.hpp"
#include "localization_language.hpp"

int WTextCharacter::calculateColorOffset( WTextCharacter::Color color )
{
    return ( color == Color::__NULL ) ? 0 : ( int )( color ) * Localization::getCurrentLanguage().getCharsetHeight() * WTextCharacter::SIZE_PIXELS;
};

void WTextCharacter::changeColorOffset( int color_offset )
{
    int fundamental_y = y_ % ( SIZE_PIXELS * Localization::getCurrentLanguage().getCharsetHeight() );
    y_ = fundamental_y += color_offset;
};

void WTextCharacter::changeColor( WTextCharacter::Color color )
{
    changeColorOffset( calculateColorOffset( color ) );
};