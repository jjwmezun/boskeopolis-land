#pragma once

#include "direction.hpp"
#include <string>
#include "unit.hpp"

class Clock final
{
	public:	
		constexpr Clock
		(
			int start_time = 0,
			Direction::Vertical direction = DEFAULT_DIRECTION,
			int limit = DEFAULT_LIMIT
		)
		:
			frames_ ( start_time * Unit::FPS ),
			limit_ ( limit * Unit::FPS ),
			direction_ ( direction ),
			on_ ( true ),
			changed_ ( true )
		{};

		bool update();
		void reset( Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
		void setCountdown( int limit );
		void stop();
		void addTime( int seconds );
		std::u32string getTimeString() const;
		static std::u32string timeToString( int total_seconds );

		constexpr int secondsFromTotal() const
		{
			return secondsFromTotal( totalSeconds() );
		};

		constexpr int minutesFromTotalSeconds() const
		{
			return minutesFromTotalSeconds( totalSeconds() );
		};

		constexpr int totalSeconds( int frames ) const
		{
			return floor( frames / Unit::FPS );
		};

		static constexpr int secondsFromTotal( int total_seconds )
		{
			return total_seconds % SECONDS_PER_MINUTE;
		};

		static constexpr int minutesFromTotalSeconds( int total_seconds )
		{
			return floor( total_seconds / SECONDS_PER_MINUTE );
		};

		constexpr int totalSeconds() const
		{
			return totalSeconds( frames_ );
		};

		constexpr bool hitLimit() const
		{
			return frames_ >= limit_;
		};

		constexpr bool lowOnTime() const
		{
			return direction_ == Direction::Vertical::DOWN && secondsRemaining() < 5;
		};

		constexpr int secondsRemaining() const
		{
			return ( int )( ceil( ( double )( limit_ - frames_ ) / ( double )( Unit::FPS ) ) );
		};
		
		constexpr int countdownHit0() const
		{
			return frames_ >= limit_;
		};

	private:
		static constexpr int DEFAULT_LIMIT = 599;
		static constexpr Direction::Vertical DEFAULT_DIRECTION = Direction::Vertical::UP;
		static constexpr int SECONDS_PER_MINUTE = 60;

		int frames_;
		int limit_;
		Direction::Vertical direction_;
		bool on_;
		bool changed_;
};