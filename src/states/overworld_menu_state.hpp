#pragma once

#include <array>
#include "game_state.hpp"
#include "mezun_sdl2.hpp"
#include "ow_state.hpp"
#include "text_obj.hpp"

class OverworldMenuState : public GameState
{
	public:
		OverworldMenuState( const Palette& pal, OWState* camera_state );
		~OverworldMenuState();
		void stateUpdate() override;
		void stateRender() override;
		void backFromPop() override;

		enum class Option
		{
			CONTINUE,
			LIST,
			CAMERA,
			OPTIONS,
			QUIT
		};
		static constexpr int NUM_O_OPTIONS = ( int )( Option::QUIT ) + 1;

	private:
		int option_selection_;
		OWState* camera_state_;
		std::array<TextObj, NUM_O_OPTIONS> option_text_;
		const sdl2::SDLRect bg_;
};
