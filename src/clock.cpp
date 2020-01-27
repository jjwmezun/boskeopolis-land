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

std::string Clock::timeToString2( int total_seconds )
{
	return Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) );
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
	return ( direction_ == Direction::Vertical::DOWN ) 
		? mezun::intToChar32String( minutesFromTotalSeconds( timeRemaining() ) ) + U":" + mezun::intToChar32StringWithPadding( timeRemaining(), 2 )
		: mezun::intToChar32String( minutesFromTotalSeconds() ) + U":" + mezun::intToChar32StringWithPadding( secondsFromTotal(), 2 );
};