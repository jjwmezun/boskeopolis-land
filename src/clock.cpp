#include "clock.hpp"
#include <cmath>
#include "unit.hpp"

void Clock::update()
{
	if ( on_ )
	{
		if ( frames_timer_ >= Unit::FPS )
		{
			frames_timer_ = 0;
			++total_seconds_;

			if ( total_seconds_ > limit_ )
			{
				total_seconds_ = limit_;
			}
		}
		++frames_timer_;
	}
};

void Clock::renderTime( int x, int y, int total_seconds, const Camera* camera, Text::FontColor color, Text::FontAlign align, Text::FontColor shadow, int magnification )
{
	Text::renderText( Text::timeToString( secondsFromTotal( total_seconds ), minutesFromTotalSeconds( total_seconds ) ), x, y, camera, color, Text::DEFAULT_LINE_LENGTH, align, shadow, magnification );
}

std::string Clock::timeToString( int total_seconds )
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