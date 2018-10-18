#pragma once

#include "game_state.hpp"
#include "text_obj.hpp"

class LevelMessageState : public GameState
{
	public:
		LevelMessageState( const Palette& palette, std::string message );
		~LevelMessageState();
		void stateUpdate() override;
		void stateRender() override;

		static constexpr int MESSAGE_BOX_WIDTH_MINIBLOCKS = 44;
		static constexpr int MESSAGE_BOX_PADDING_MINIBLOCKS = 2;
		static constexpr int LINE_LIMIT = MESSAGE_BOX_WIDTH_MINIBLOCKS - ( MESSAGE_BOX_PADDING_MINIBLOCKS * 2 );

	private:
		const sdl2::SDLRect backdrop_;
		const sdl2::SDLRect border_;
		TextObj message_;
};
