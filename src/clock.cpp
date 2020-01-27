#include "clock.hpp"
#include <cmath>
#include "mezun_helpers.hpp"
#include "unit.hpp"

bool Clock::update()
{
	if ( on_ )
	{
		++frames_timer_;
		if ( frames_timer_ == Unit::FPS )
		{
			frames_timer_ = 0;
			if ( total_seconds_ < limit_ )
			{
				++total_seconds_;
				return true;
			}
		}
	}
	return false;
};

void Clock::renderTime( int x, int y, int total_seconds, const Camera* camera, Text::FontColor color, Text::FontAlign align, Text::FontColor shadow, int magnification )
{
	Text::renderText( Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) ), x, y, camera, color, Text::DEFAULT_LINE_LENGTH, align, shadow, magnification );
}

std::string Clock::timeToString2( int total_seconds )
{
	return Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) );
};

void Clock::render( int x, int y, const Camera* camera, Text::FontColor color, Text::FontAlign align, Text::FontColor shadow, int magnification ) const
{
	if ( direction_ == Direction::Vertical::DOWN )
	{
		renderTime( x, y, timeRemaining(), camera, color, align, shadow, magnification );
	}
	else
	{
		renderTime( x, y, total_seconds_, camera, color, align, shadow, magnification );
	}
};

void Clock::reset( Direction::Vertical direction, int limit )
{
	frames_timer_ = 0;
	total_seconds_ = 0;
	limit_ = limit;
	direction_ = direction;
	on_ = true;
};

void Clock::startMoonCountdown( int start_time )
{
	direction_ = Direction::Vertical::DOWN;
	limit_ = total_seconds_ + start_time;
};

void Clock::stop()
{
	on_ = false;
};

std::u32string Clock::getTimeString() const
{
	return mezun::intToChar32String( minutesFromTotalSeconds() ) + U":" + mezun::intToChar32StringWithPadding( secondsFromTotal(), 2 );
};