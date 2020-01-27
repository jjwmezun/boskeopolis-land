#pragma once

#include "direction.hpp"
#include <string>
#include "text.hpp"

class Clock
{
	public:	
		constexpr Clock
		(
			int start_time = 0,
			Direction::Vertical direction = DEFAULT_DIRECTION,
			int limit = DEFAULT_LIMIT
		)
		:
			frames_timer_ ( 0 ),
			total_seconds_ ( start_time ),
			limit_ ( limit ),
			direction_ ( direction ),
			on_ ( true )
		{};

		bool update();
		void reset( Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
		void startMoonCountdown( int start_time );
		void stop();
		std::u32string getTimeString() const;
		static std::string timeToString2( int total_seconds );

		constexpr int secondsFromTotal() const
		{
			return secondsFromTotal( total_seconds_ );
		};

		constexpr int minutesFromTotalSeconds() const
		{
			return minutesFromTotalSeconds( total_seconds_ );
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
			return total_seconds_;
		};

		constexpr bool hitLimit() const
		{
			return total_seconds_ >= limit_;
		};

		constexpr bool lowOnTime() const
		{
			return direction_ == Direction::Vertical::DOWN && timeRemaining() < 5;
		};

		constexpr int timeRemaining() const
		{
			return limit_ - total_seconds_;
		};
		
		constexpr int countdownHit0() const
		{
			return timeRemaining() <= 0;
		};

	private:
		static constexpr int DEFAULT_LIMIT = 599;
		static constexpr Direction::Vertical DEFAULT_DIRECTION = Direction::Vertical::UP;
		static constexpr int SECONDS_PER_MINUTE = 60;

		int frames_timer_;
		int total_seconds_;
		int limit_;
		Direction::Vertical direction_;
		bool on_;
};