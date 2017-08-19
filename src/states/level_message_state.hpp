#pragma once

#include "game_state.hpp"
#include "text.hpp"

class LevelMessageState : public GameState
{

	public:
		LevelMessageState( const Palette& palette, std::string message );
		~LevelMessageState();

		void update();
		void stateRender();
		void init();
		void backFromPop();


	private:
		static constexpr int TEXT_X = 32;
		static constexpr int TEXT_Y = 32;
		static constexpr int MESSAGE_BOX_PADDING = 2;
		static constexpr int MESSAGE_BOX_WIDTH = Unit::WINDOW_WIDTH_PIXELS - ( ( TEXT_X - Unit::MiniBlocksToPixels( MESSAGE_BOX_PADDING ) ) * 2 );
		static constexpr int MESSAGE_BOX_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - ( ( TEXT_Y - Unit::MiniBlocksToPixels( MESSAGE_BOX_PADDING ) ) * 2 );
		static constexpr int LINE_LIMIT = Unit::PixelsToMiniBlocks( MESSAGE_BOX_WIDTH ) - ( MESSAGE_BOX_PADDING * 2 );
		const Text message_;
		static constexpr sdl::rect backdrop_ =
		{
			( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( MESSAGE_BOX_WIDTH / 2 ),
			( Unit::WINDOW_HEIGHT_PIXELS / 2 ) - ( MESSAGE_BOX_HEIGHT / 2 ),
			MESSAGE_BOX_WIDTH,
			MESSAGE_BOX_HEIGHT
		};

};