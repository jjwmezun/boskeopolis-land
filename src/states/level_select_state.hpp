#pragma once

#include "direction.hpp"
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
		void generatePageTextures();
		void turnOffAndResetAnimations();
		void renderSelectHighlight() const;
		WTextCharacter::Color flashOnCondition( bool condition ) const;
		void renderFlashingPtSymbol( int y ) const;
		void renderFlashingClockSymbol( int y ) const;
		void renderFlashingLevelName( int level, int cycle, int y ) const;
		void renderFlashingThemeIcon( int theme, int y ) const;
		void renderFlashingVictoryCheck( int y ) const;
		void renderFlashingDiamondWinIcon( int y ) const;
		void renderFlashingGemScore( int level, int y ) const;
		void renderFlashingTimeScore( int level, int y ) const;
		static int calculateNumberOfLevelsInPage( int page );

		static constexpr int PAGES_PER_CYCLE = 2;
		static constexpr int NUMBER_OF_PAGES = PAGES_PER_CYCLE * Level::NUMBER_OF_CYCLES;
		static constexpr int LEVELS_PER_PAGE = 8;
		static constexpr int NUMBER_OF_FLASH_FRAMES = 12;
		static constexpr int FLASH_FRAMES[ NUMBER_OF_FLASH_FRAMES ] =
		{
			0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0
		};

		WTextObj title_;
		sdl2::SDLRect screen_;
		sdl2::SDLRect back_position_;
		TextureBox* current_page_texture_;
		TextureBox* next_page_texture_;
		int back_position_timer_;
		int selection_timer_;
		int selection_;
		int page_timer_;
		int page_;
		int flash_timer_;
		int flash_frame_;
		int title_character_;
		int title_highlight_timer_;
		Direction::Horizontal page_change_direction_;
		int next_page_;
		TextureBox page_textures_[ NUMBER_OF_PAGES ];
		char scratch_[ 1200 ];

		
};
