#pragma once

#include "direction.hpp"
#include "game_state.hpp"
#include "level.hpp"
#include "mezun_math.hpp"
#include "wtext_obj.hpp"

class LevelSelectState : public GameState
{
	public:
		static constexpr int LEVELS_PER_PAGE = 8;
		static constexpr int PAGES_PER_CYCLE = mezun::ceil( ( double )( Level::NUMBER_OF_THEMES ) / ( double )( LEVELS_PER_PAGE ) );
		static constexpr int NUMBER_OF_PAGES = PAGES_PER_CYCLE * Level::NUMBER_OF_CYCLES;

		LevelSelectState( int current_level );
		~LevelSelectState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		void renderBackground() const;
		void flipPage( Direction::Horizontal direction, int next_page, int position );
		void flipPageLeft();
		void flipPageRight();
		void finishPageFlip();
		void handleTargetScoreBehavior();
		void setInfoToNextPage();
		void redrawStaticPageTexture();
		void drawFlippedTexture();
		void animateTextFlashColor();
		void updateBackgroundAnimation();
		void animateTitleHighlight();
		void updateTitleHighlightGraphics();
		void generatePageTextures();
		void generateMovingPageTextures();
		void turnOffAndResetAnimations();
		void renderSelectHighlight() const;
		WTextCharacter::Color flashOnCondition( bool condition ) const;
		void renderFlashingPtSymbol( int y ) const;
		void renderFlashingClockSymbol( int y ) const;
		void renderLevelNameOfColor( WTextCharacter::Color color, int level );
		void renderLevelName( int level );
		void renderFlashingLevelName( int level );
		void renderFlashingThemeIcon( int theme, int y ) const;
		void renderFlashingVictoryCheck( int y ) const;
		void renderFlashingDiamondWinIcon( int y ) const;
		void renderFlashingGemScore( int level, int y ) const;
		void renderFlashingTimeScore( int level, int y ) const;
		void renderFlashingArrows() const;
		void renderFlashFrame();
		void generateLevelNames();

		WTextObj level_names_[ Level::NUMBER_OF_LEVELS ];
		TextureBox moving_page_textures_[ NUMBER_OF_PAGES ];
		WTextObj title_;
		sdl2::SDLRect screen_;
		sdl2::SDLRect background_position_;
		TextureBox static_page_texture_;
		TextureBox final_table_texture_;
		TextureBox* current_page_texture_;
		TextureBox* next_page_texture_;
		int background_position_timer_;
		int selection_timer_;
		int selection_;
		int page_;
		int flash_timer_;
		int flash_frame_;
		int title_character_;
		int title_highlight_timer_;
		int next_page_;
		int first_level_of_page_;
		int number_of_pages_;
		Direction::Horizontal page_change_direction_;
		bool show_target_scores_;

		
};
