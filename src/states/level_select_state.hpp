#pragma once

#include "game_state.hpp"

class LevelSelectState : public GameState
{
	public:
		LevelSelectState( int current_level );
		~LevelSelectState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		sdl2::SDLRect screen_;
		sdl2::SDLRect back_position_;
		int back_position_timer_;
		int selection_timer_;
		int selection_;
		int page_;
		char scratch_[ 1900 ];
};
