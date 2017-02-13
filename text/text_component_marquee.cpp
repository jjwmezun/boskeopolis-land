


// Name
//===================================
//
// TextComponentMarquee
//


// DEPENDENCIES
//===================================

    #include "text.h"
    #include "text_component_marquee.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TextComponentMarquee::TextComponentMarquee
    (
        Direction::Horizontal direction,
        int speed
    )
    :
        direction_ ( direction ),
        speed_ ( speed )
    {};

    TextComponentMarquee::~TextComponentMarquee() {};

    void TextComponentMarquee::update( Text& text )
    {
        if ( direction_ == Direction::Horizontal::RIGHT )
        {
            text.moveRight( speed_ );
        }
        else
        {
            text.moveLeft( speed_ );
        }
    };
