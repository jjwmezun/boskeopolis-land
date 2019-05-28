#pragma once

#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "unit.hpp"

class OWInventory
{
	public:
		OWInventory();
		void update( int lv_select );
		void render( int lv_select );

	private:
		static constexpr sdl2::SDLRect AREA = { 0, Unit::WINDOW_HEIGHT_PIXELS - 40, Unit::WINDOW_WIDTH_PIXELS, 40 };
		static constexpr int ROW_1 = AREA.y + 8;
		static constexpr int ROW_2 = AREA.y + 24;
		static constexpr int LEFT_EDGE = AREA.x + 8;
		static constexpr int RIGHT_EDGE = AREA.w - 8;
		static constexpr int COLOR_START = 1;
		static constexpr int COLOR_END   = 3;
		static constexpr int COLOR_TIMER_LIMIT = 4;
		static constexpr int HAVE_X = 48;
		static constexpr int DONT_HAVE_X = 56;

		void renderLevelInfo( int lv_select );
		void renderPts();

		SpriteGraphics win_icon_gfx_;
		SpriteGraphics diamond_gfx_;
		SpriteGraphics gem_icon_gfx_;
		SpriteGraphics time_icon_gfx_;
		TextObj level_name_;
		sdl2::SDLRect area_;
		sdl2::SDLRect win_icon_pos_;
		sdl2::SDLRect diamond_icon_pos_;
		sdl2::SDLRect gem_icon_pos_;
		sdl2::SDLRect time_icon_pos_;
		sdl2::SDLRect pts_icon_pos_;
		int color_animation_;
		int color_animation_timer_;
		int prev_level_select_;
		bool show_challenges_;
		bool show_challenges_lock_;
		bool color_going_up_;
		bool sound_lock_;
};
