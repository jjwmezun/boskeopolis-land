#pragma once

#include "counter_flip.hpp"
#include "frame.hpp"
#include "game_state.hpp"
#include "mezun_sdl2.hpp"
#include "ow_state.hpp"
#include "ow_tile.hpp"
#include "wtext_obj.hpp"

class OverworldMenuState final : public GameState
{
	public:
		OverworldMenuState( const Palette& pal, OWState* camera_state, OWTile space, int level_color );
		~OverworldMenuState();
		void init() override;
		void stateUpdate() override;
		void stateRender() override;
		void backFromPop() override;

		enum class Option
		{
			CONTINUE,
			LIST,
			CAMERA,
			CARDS,
			OPTIONS,
			QUIT
		};
		static constexpr int NUM_O_OPTIONS = ( int )( Option::QUIT ) + 1;

	private:
		void renderOptionsText();
		void regenerateOptionsTextOnLanguageChange();
		void generateOptionsText();
		void generateReplacementLevelNameTexture();
		void generateReplacementShopNameTexture();
		void generateReplacementNameTexture( const std::u32string& string, WTextCharacter::Color color );

		int language_id_;
		int level_color_;
		CounterFlip<NUM_O_OPTIONS - 1> option_selection_;
		OWState* camera_state_;
		OWTile space_;
		TextureBox level_name_replacement_;
		WTextObj options_text_[ NUM_O_OPTIONS ];
		Frame frame_;
};
