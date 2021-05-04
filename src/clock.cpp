#include "clock.hpp"
#include <cmath>
#include "mezun_helpers.hpp"
#include "unit.hpp"

bool Clock::update()
{
	bool local_changed = changed_;
	changed_ = false;
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
	frames_timer_ = 0;
	total_seconds_ = 0;
	limit_ = limit;
	direction_ = direction;
	on_ = true;
	changed_ = true;
};

void Clock::stop()
{
	on_ = false;
};

void Clock::addTime( int seconds )
{
	if ( direction_ == Direction::Vertical::DOWN )
	{
		limit_ += seconds;
	}
	else
	{
		total_seconds_ -= seconds;
		if ( total_seconds_ < 0 )
		{
			total_seconds_ = 0;
		}
	}
	changed_ = true;
};

std::u32string Clock::getTimeString() const
{
	return ( direction_ == Direction::Vertical::DOWN ) 
		? mezun::intToChar32String( minutesFromTotalSeconds( timeRemaining() ) ) + U":" + mezun::intToChar32StringWithPadding( secondsFromTotal( timeRemaining() ), 2 )
		: timeToString( total_seconds_ );
};