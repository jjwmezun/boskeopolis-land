#pragma once

#include "game_state.h"
#include "mezun_sdl2.h"
#include "text.h"
#include <vector>

class OverworldMenuState : public GameState
{

	public:
		OverworldMenuState();
		~OverworldMenuState();

		void update( Game& game, const Input& input, Graphics& graphics );
		void stateRender( Graphics& graphics );
		void init( Game& game, Graphics& graphics );

	private:
		enum class Option
		{
			CONTINUE,
			QUIT
		};
		static constexpr int NUM_O_OPTIONS = ( int )Option::QUIT + 1;

		static constexpr int PAUSE_BOX_WIDTH = 24;
		static constexpr int PAUSE_BOX_HEIGHT = 7;
		static constexpr sdl2::SDLRect bg_ =
		{
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_WIDTH / 2 ) ),
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_HEIGHT / 2 ) ),
			Unit::MiniBlocksToPixels( PAUSE_BOX_WIDTH ),
			Unit::MiniBlocksToPixels( PAUSE_BOX_HEIGHT )
		};

		const std::array<Text, NUM_O_OPTIONS> option_text_ =
		{
			Text( "Continue", bg_.x + 8, bg_.y + 16, Text::FontShade::LIGHT_MID_GRAY ),
			Text( "Quit", bg_.x + 8, bg_.y + 32, Text::FontShade::LIGHT_MID_GRAY )
		};
		int option_selection_;
};