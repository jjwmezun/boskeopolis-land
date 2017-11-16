#pragma once

#include "game_state.hpp"
#include "text.hpp"

class LevelMessageState : public GameState
{
	public:
		LevelMessageState( const Palette& palette, std::string message );
		~LevelMessageState();
		void stateUpdate() override;
		void stateRender() override;

	private:
		const sdl2::SDLRect backdrop_;
		const Text message_;
};