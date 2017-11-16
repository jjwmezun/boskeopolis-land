#pragma once

#include "game_state.hpp"

class TimeStartState : public GameState
{
	public:
		TimeStartState( const Palette& palette );
		~TimeStartState();

		void stateUpdate() override;
		void stateRender() override;

	private:
		enum class State
		{
			GOING_DOWN,
			BUMPING_UPWARD,
			BUMPING_BACK_DOWN,
			BLINKING,
			SHRINKING,
			DONE
		};

		int y_;
		int x_;
		int size_;
		int timer_;
		Text::FontColor color_;
		State state_;
};