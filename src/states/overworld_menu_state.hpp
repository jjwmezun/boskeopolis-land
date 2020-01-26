#pragma once

#include "counter_flip.hpp"
#include "game_state.hpp"
#include "image_graphics.hpp"
#include "mezun_sdl2.hpp"
#include "ow_state.hpp"
#include "wtext_obj.hpp"

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
		CounterFlip<NUM_O_OPTIONS - 1> option_selection_;
		OWState* camera_state_;
		WTextObj options_text_[ NUM_O_OPTIONS ];
		const sdl2::SDLRect bg_;
		ImageGraphics frame_;
};
