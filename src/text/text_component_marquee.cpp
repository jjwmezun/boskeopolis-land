#include "text_obj.hpp"
#include "text_component_marquee.hpp"

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

void TextComponentMarquee::update( TextObj& text )
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

void TextComponentMarquee::moveLeft( TextObj& text )
{
	text.x_ -= speed_;

	if ( text.right() < 0 )
	{
		text.x_ = Unit::WINDOW_WIDTH_PIXELS;
	}
};

void TextComponentMarquee::moveRight( TextObj& text )
{
	text.x_ += speed_;

	if ( text.x_ > Unit::WINDOW_WIDTH_PIXELS )
	{
		text.x_ = -text.right();
	}
};

void TextComponentMarquee::overrideLineLimit( TextObj& text )
{
	text.line_limit_ = text.words_.length();
};