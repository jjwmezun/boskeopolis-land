#pragma once

#include <string>
#include "text.hpp"
#include "timers/timer_repeat.hpp"
#include "unit.hpp"

class Clock
{
	public:
		Clock( int start_time = 0, Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
		~Clock();

		void render
		(
			int x,
			int y,
			Camera* camera = nullptr,
			Text::FontShade color = Text::FontShade::BLACK,
			Text::FontAlign align = Text::FontAlign::LEFT,
			Text::FontShade shadow = Text::FontShade::__NULL,
			int magnification = 1
		) const;

		static void renderTime
		(
			int x,
			int y,
			int total_seconds,
			Camera* camera = nullptr,
			Text::FontShade color = Text::FontShade::BLACK,
			Text::FontAlign align = Text::FontAlign::LEFT,
			Text::FontShade shadow = Text::FontShade::__NULL,
			int magnification = 1
		);

		void update();
		void reset( Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
		int secondsFromTotal() const;
		int minutesFromTotalSeconds() const;
		static int secondsFromTotal( int total_seconds );
		static int minutesFromTotalSeconds( int total_seconds );
		static std::string timeToString( int total_seconds );
		int totalSeconds() const;
		bool hitLimit() const;
		bool lowOnTime() const;
		int timeRemaining() const;

	private:
		static constexpr int DEFAULT_LIMIT = 599;
		static constexpr Direction::Vertical DEFAULT_DIRECTION = Direction::Vertical::UP;
		static constexpr int SECONDS_PER_MINUTE = 60;

		int total_seconds_;
		int limit_;
		Direction::Vertical direction_;
		TimerRepeat timer_ = TimerRepeat( Unit::FPS );
};