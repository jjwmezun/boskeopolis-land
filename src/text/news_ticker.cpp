#include <cassert>
#include <iostream>
#include "mezun_helpers.hpp"
#include "news_ticker.hpp"
#include "text_component_marquee.hpp"

NewsTicker::NewsTicker( int y )
:
	Text
	(
		"",
		Unit::WINDOW_WIDTH_PIXELS,
		y,
		FontColor::DARK_GRAY,
		FontAlign::LEFT,
		false,
		NULL, 
		std::unique_ptr<TextComponent>
		(
			new TextComponentMarquee( Direction::Horizontal::LEFT, 1 )
		) 
	)
{
	changeMessage();
};

void NewsTicker::updateTicker()
{
	// Check just before position changes from marquee.
	if ( right() == 0 )
	{
		changeMessage();
	}

	update();
};

std::string NewsTicker::newMessage() const
{
	return mezun::randomListItem( strings_ );
};

void NewsTicker::changeMessage()
{
	words_ = newMessage();
	x_ = -width(); // Readjust position so still left o' screen with new string size.
};