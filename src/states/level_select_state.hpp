#pragma once

#include "game_state.hpp"
#include "wtext_obj.hpp"

class LevelSelectState : public GameState
{
	public:
		LevelSelectState( int current_level );
		~LevelSelectState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		WTextObj::Color flashOnCondition( bool condition ) const;

		sdl2::SDLRect screen_;
		sdl2::SDLRect back_position_;
		int back_position_timer_;
		int selection_timer_;
		int selection_;
		int page_;
		int flash_timer_;
		int flash_frame_;
		char scratch_[ 1832 ];
};
