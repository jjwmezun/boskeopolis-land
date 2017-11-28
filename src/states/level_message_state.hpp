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

	private:
		const sdl2::SDLRect backdrop_;
		const sdl2::SDLRect border_;
		TextObj message_;
};