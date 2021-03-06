#pragma once

#include "image_graphics.hpp"
#include "ow_tile.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"
#include "unit.hpp"
#include "wtext_obj.hpp"

class OWInventory final
{
	public:
		// From dark to light, delaying a li’l @ either end.
		static constexpr int NUMBER_OF_FLASH_FRAMES = 10;
		static constexpr int FLASH_FRAMES[ NUMBER_OF_FLASH_FRAMES ] =
		{
			0, 1, 2, 3, 4, 4, 3, 2, 1, 0
		};

		static constexpr int HEIGHT = 40;

		OWInventory();
		~OWInventory();
		OWInventory( const OWInventory& ) = delete;
		OWInventory( OWInventory&& ) = delete;
		OWInventory& operator=( const OWInventory& ) = delete;
		OWInventory& operator=( OWInventory&& ) = delete;
		void update( OWTile space );
		void render();
		void init();
		void forceLevelNameRedraw();

		inline int getFlashColor() const
		{
			return FLASH_FRAMES[ color_animation_ ];
		};

	private:
		void renderLevelInfo();
		void renderPts();
		void regenerateLevelGraphics();
		void updateFlashColor();
		void updateShowChallengeScoresInput();
		void setShowChallengesOn();
		void setShowChallengesOff();
		void updateTextFlashColor();
		bool testOnDifferentLevel() const;
		bool testMoneyInTheRed() const;
		void regenerateLevelNameGraphics();
		WTextObj generateName( std::u32string text, int offset ) const;
		void regenerateShopGraphics();

		bool show_challenges_;
		bool show_challenges_lock_;
		bool sound_lock_;
		int color_animation_;
		int color_animation_timer_;
		TextureBox* current_gem_score_texture_;
		TextureBox* current_time_score_texture_;
		OWTile previous_space_;
		OWTile space_;
		TextureBox name_textures_[ ( int )( WTextCharacter::Color::__NULL ) ];
		TextureBox gem_score_textures_[ ( int )( WTextCharacter::Color::__NULL ) ];
		TextureBox time_score_textures_[ ( int )( WTextCharacter::Color::__NULL ) ];
		TextureBox gem_score_target_texture_;
		TextureBox time_score_target_texture_;
		ImageGraphics bg_frame_;
		ImageGraphics negative_pts_icon_;
		ImageGraphics win_icon_;
		ImageGraphics diamond_icon_;
		ImageGraphics gem_score_icon_;
		ImageGraphics time_score_icon_;
		ImageGraphics suits_icon_;
		ImageGraphics secret_goal_icon_;
		ImageGraphics level_icon_;
};
