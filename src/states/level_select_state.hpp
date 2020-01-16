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

		WTextObj title_;
		sdl2::SDLRect screen_;
		sdl2::SDLRect back_position_;
		TextureBox current_page_;
		TextureBox previous_page_;
		int back_position_timer_;
		int selection_timer_;
		int selection_;
		int page_;
		int flash_timer_;
		int flash_frame_;
		int title_character_;
		int title_highlight_timer_;
		char scratch_[ 1500 ];
};
