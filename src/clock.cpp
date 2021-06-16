#include "clock.hpp"
#include <cmath>
#include "mezun_helpers.hpp"

bool Clock::update()
{
	bool local_changed = changed_;
	changed_ = false;
	if ( on_ )
	{
		++frames_;
		if ( frames_ % Unit::FPS == 0 )
		{
			return changed_ = true;
		}
	}
	return local_changed;
};

std::u32string Clock::timeToString( int total_seconds )
{
	return mezun::intToChar32String( minutesFromTotalSeconds( total_seconds ) ) +
		U":" +
		mezun::intToChar32StringWithPadding( secondsFromTotal( total_seconds ), 2 );
};

void Clock::reset( Direction::Vertical direction, int limit )
{
	frames_ = 0;
	limit_ = limit * Unit::FPS;
	direction_ = direction;
	on_ = changed_ = true;
};

void Clock::setCountdown( int limit )
{
	limit_ = frames_ + limit * Unit::FPS;
	direction_ = Direction::Vertical::DOWN;
	on_ = changed_ = true;
};

void Clock::stop()
{
	on_ = false;
};

void Clock::addTime( int seconds )
{
	if ( direction_ == Direction::Vertical::DOWN )
	{
		limit_ += seconds * Unit::FPS;
	}
	else
	{
		frames_ -= seconds * Unit::FPS;
		if ( frames_ < 0 )
		{
			frames_ = 0;
		}
	}
	changed_ = true;
};

std::u32string Clock::getTimeString() const
{
	return ( direction_ == Direction::Vertical::DOWN ) 
		? mezun::intToChar32String( minutesFromTotalSeconds( secondsRemaining() ) ) + U":" + mezun::intToChar32StringWithPadding( secondsFromTotal( secondsRemaining() ), 2 )
		: timeToString( totalSeconds() );
};