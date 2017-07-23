#pragma once

#include "event_system.h"
#include "game_state.h"
#include "inventory.h"
#include "mezun_sdl2.h"
#include "text.h"
#include <vector>

class OverworldMenuState : public GameState
{

	public:
		OverworldMenuState( bool& go_to_list, bool& camera_mode, const Palette& pal );
		~OverworldMenuState();

		void update( const Input& input );
		void stateRender();
		void init();

	private:
		enum class Option
		{
			CONTINUE,
			LIST,
			CAMERA,
			QUIT
		};
		static constexpr int NUM_O_OPTIONS = ( int )( Option::QUIT ) + 1;

		static constexpr int BG_WIDTH = 24;
		static constexpr int BG_HEIGHT = 11;
		static constexpr sdl2::SDLRect bg_ =
		{
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( BG_WIDTH / 2 ) ),
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( BG_HEIGHT / 2 ) ),
			Unit::MiniBlocksToPixels( BG_WIDTH ),
			Unit::MiniBlocksToPixels( BG_HEIGHT )
		};

		const std::array<Text, NUM_O_OPTIONS> option_text_ =
		{
			Text( "Continue", bg_.x + 8, bg_.y + 16, Text::FontShade::LIGHT_MID_GRAY ),
			Text( "Level List", bg_.x + 8, bg_.y + 32, Text::FontShade::LIGHT_MID_GRAY ),
			Text( "Camera View", bg_.x + 8, bg_.y + 48, Text::FontShade::LIGHT_MID_GRAY ),
			Text( "Quit", bg_.x + 8, bg_.y + 64, Text::FontShade::LIGHT_MID_GRAY )
		};
		int option_selection_;
		bool& go_to_list_;
		bool& camera_mode_;
};