#pragma once

#include "checkered_background.hpp"
#include "direction.hpp"
#include "game_state.hpp"
#include "level.hpp"
#include "level_select_page.hpp"
#include "mezun_math.hpp"
#include "wtext_highlight.hpp"
#include "wtext_obj.hpp"

class LevelSelectState final : public GameState
{
	public:
		static constexpr int MAX_LEVELS_PER_PAGE = 8;

		LevelSelectState( int current_level );
		~LevelSelectState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		unsigned int getSelectedLevel() const;
		void renderPageToTexture( const TextureBox& texture, const LevelSelectPage& page, bool render_select = true ) const;
		void renderPage( const LevelSelectPage& page, bool render_select = true ) const;
		void flipPage( Direction::Horizontal direction, int next_page, int position );
		void flipPageLeft();
		void flipPageRight();
		void finishPageFlip();
		void drawFlippedTexture() const;
		void redrawCurrentPage() const;
		void animateTextFlashColor();
		void renderFlashingArrows() const;
		void handleTargetScoreBehavior();
		void updatePalette();
		void resetInstructionPosition();

		bool show_target_scores_;
		Direction::Horizontal page_change_direction_;
		int selection_timer_;
		int selection_;
		int current_page_;
		int next_page_;
		int flash_timer_;
		int flash_frame_;
		double percent_;
		std::vector<LevelSelectPage> pages_;
		CheckeredBackground background_;
		TextureBox current_page_texture_;
		TextureBox next_page_texture_;
		TextureBox final_table_texture_;
		WTextObj instruction_;
		WTextObj completion_;
		WTextHighlight<> title_;
};