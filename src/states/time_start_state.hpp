#pragma once

#include "game_state.hpp"

class TimeStartState : public GameState
{
	public:
		TimeStartState( const Palette& palette );
		~TimeStartState();

		void update() override;
		void stateRender() override;
		void init() override;

	private:
		enum class State
		{
			GOING_DOWN,
			BUMPING_UPWARD,
			BUMPING_BACK_DOWN,
			DONE
		};

		int y_;
		int size_;
		int size_timer_;
		State state_;
};