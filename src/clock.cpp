#include "clock.hpp"
#include <cmath>

Clock::Clock
(
	int start_time,
	Direction::Vertical direction,
	int limit
)
:
	total_seconds_ ( start_time ),
	limit_ ( limit ),
	direction_ ( direction )
{};

Clock::~Clock() {};

void Clock::update()
{
	if ( timer_.hit() )
	{
		++total_seconds_;

		if ( total_seconds_ > limit_ )
		{
			total_seconds_ = limit_;
		}
	}

	timer_.update();
};

int Clock::secondsFromTotal() const
{
	return secondsFromTotal( total_seconds_ );
};

int Clock::minutesFromTotalSeconds() const
{
	return minutesFromTotalSeconds( total_seconds_ );
};

int Clock::secondsFromTotal( int total_seconds )
{
	return total_seconds % SECONDS_PER_MINUTE;
};

int Clock::minutesFromTotalSeconds( int total_seconds )
{
	return floor( total_seconds / SECONDS_PER_MINUTE );
};

int Clock::totalSeconds() const
{
	return total_seconds_;
};

bool Clock::hitLimit() const
{
	return total_seconds_ >= limit_;
};

void Clock::renderTime( int x, int y, int total_seconds, Camera* camera, Text::FontShade color, Text::FontAlign align, Text::FontShade shadow, int magnification )
{
	Text::renderText( Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) ), x, y, camera, color, Text::DEFAULT_LINE_LENGTH, align, shadow, magnification );
}

std::string Clock::timeToString( int total_seconds )
{
	return Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) );
};

void Clock::render( int x, int y, Camera* camera, Text::FontShade color, Text::FontAlign align, Text::FontShade shadow, int magnification ) const
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
	*this = Clock( 0, direction, limit );
};

bool Clock::lowOnTime() const
{
	return direction_ == Direction::Vertical::DOWN && timeRemaining() < 5;
};

int Clock::timeRemaining() const
{
	return limit_ - total_seconds_;
};