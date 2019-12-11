#include "char_frame.hpp"
#include "localization.hpp"
#include "localization_language.hpp"

int CharFrame::changeColorOffset( int color_offset )
{
    int fundamental_y = y_ % ( SIZE_PIXELS * Localization::getCurrentLanguage().getCharsetHeight() );
    y_ = fundamental_y += color_offset;
};