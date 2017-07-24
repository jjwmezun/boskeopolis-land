#pragma once

class Health;

#include "sprite_graphics.hpp"

class OxygenMeter
{
	public:
		OxygenMeter( int y );
		void update( const Health& health );
		void render() const;


	private:
		static constexpr int WIDTH_MINIBLOCKS = 10;
		static constexpr int HEIGHT_MINIBLOCKS = 1;
		static constexpr int WIDTH_PIXELS = Unit::MiniBlocksToPixels( WIDTH_MINIBLOCKS );
		static constexpr int HEIGHT_PIXELS = Unit::MiniBlocksToPixels( HEIGHT_MINIBLOCKS );
		static constexpr int X_PIXELS = Unit::WINDOW_WIDTH_PIXELS - WIDTH_PIXELS - Unit::MiniBlocksToPixels( 1 );

		static constexpr int FIRST_BLOCK = 0;
		static constexpr int LAST_BLOCK = WIDTH_MINIBLOCKS - 1;

		const SpriteGraphics gfx_left_bar_;
		const SpriteGraphics gfx_middle_bar_;
		const SpriteGraphics gfx_right_bar_;

		sdl2::SDLRect meter_bar_;

		bool show_;
		int color_;

		void color( double percent );
		void renderMeter() const;
		void renderShell() const;
};