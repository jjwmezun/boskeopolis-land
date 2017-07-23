#pragma once

#include "sprite_graphics.hpp"
#include "unit.hpp"

class OWInventory
{
	public:
		void update( int lv_select );
		void render( int lv_select );

	private:
		static constexpr sdl2::SDLRect AREA = { 0, Unit::WINDOW_HEIGHT_PIXELS - 32, Unit::WINDOW_WIDTH_PIXELS, 32 };
		static constexpr int ROW_1 = AREA.y + 8;
		static constexpr int ROW_2 = AREA.y + 16;
		static constexpr int LEFT_EDGE = AREA.x + 8;
		static constexpr int RIGHT_EDGE = AREA.w - 8;
	
		static constexpr int COLOR_START = 1;
		static constexpr int COLOR_END   = 3;
		static constexpr int COLOR_TIMER_LIMIT = 4;

		SpriteGraphics win_icon_gfx_ = SpriteGraphics( "tilesets/universal.png", 40, 8 );
		SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 0 );
		SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 16 );
		SpriteGraphics time_icon_gfx_ = SpriteGraphics( "tilesets/universal.png", 40, 0 );

		bool show_challenges_ = false;
		bool show_challenges_lock_ = true;
	
		int color_animation_ = COLOR_START;
		bool color_going_up_ = true;
		int color_animation_timer_ = 0;
};