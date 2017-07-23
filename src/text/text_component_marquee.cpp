


// Name
//===================================
//
// TextComponentMarquee
//


// DEPENDENCIES
//===================================

    #include "text.hpp"
    #include "text_component_marquee.hpp"
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
		overrideLineLimit( text );
		
        if ( direction_ == Direction::Horizontal::RIGHT )
        {
            moveRight( text );
        }
        else
        {
            moveLeft( text );
        }
    };

    void TextComponentMarquee::moveLeft( Text& text )
    {
        text.x_ -= speed_;

        if ( text.right() < 0 )
        {
            text.x_ = Unit::WINDOW_WIDTH_PIXELS;
        }
    };

    void TextComponentMarquee::moveRight( Text& text )
    {
        text.x_ += speed_;

        if ( text.x_ > Unit::WINDOW_WIDTH_PIXELS )
        {
            text.x_ = -text.right();
        }
    };


	void TextComponentMarquee::overrideLineLimit( Text& text )
	{
		text.line_limit_ = text.words_.length();
	};