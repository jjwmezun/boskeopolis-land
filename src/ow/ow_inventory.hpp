#pragma once

#include "image_graphics.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "unit.hpp"
#include "wtext_obj.hpp"

class OWInventory
{
	public:
		static constexpr int HEIGHT = 40;

		OWInventory();
		void update( int level );
		void render();

	private:
		void renderLevelInfo();
		void renderPts();

		int color_animation_;
		int color_animation_timer_;
		int prev_level_;
		bool show_challenges_;
		bool show_challenges_lock_;
		bool color_going_up_;
		bool sound_lock_;
		int level_;
		ImageGraphics negative_pts_icon_;
		ImageGraphics win_icon_;
		ImageGraphics diamond_icon_;
		WTextObj level_name_;
		char scratch_[ 3000 ];
};
