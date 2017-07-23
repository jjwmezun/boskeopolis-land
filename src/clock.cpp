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
	{/*
		if ( direction_ == Direction::Vertical::DOWN )
		{
			--total_seconds_;

			if ( total_seconds_ < limit_ )
				total_seconds_ = limit_;
		}
		else
		{*/
			++total_seconds_;

			if ( total_seconds_ > limit_ )
				total_seconds_ = limit_;
		//}
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

void Clock::renderTime( int x, int y, int total_seconds, Camera* camera, Text::FontShade shade )
{
	Text::renderText( Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) ), x, y, camera, shade );
}

std::string Clock::timeToString( int total_seconds )
{
	return Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) );
};

void Clock::render( int x, int y, Camera* camera, Text::FontShade shade ) const
{
	if ( direction_ == Direction::Vertical::DOWN )
	{
		renderTime( x, y, limit_ - total_seconds_, camera, shade );
	}
	else
	{
		renderTime( x, y, total_seconds_, camera, shade );
	}
};

void Clock::reset( Direction::Vertical direction, int limit )
{
	*this = Clock( 0, direction, limit );
};