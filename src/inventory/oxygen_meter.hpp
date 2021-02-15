#pragma once

class Health;

#include "sprite_graphics.hpp"

class OxygenMeter final
{
	public:
		OxygenMeter( int right, int y );
		void update( const Health& health );
		void render() const;

	private:
		void color( double percent );
		void renderMeter() const;
		void renderShell() const;

		bool show_;
		int color_;
		sdl2::SDLRect meter_bar_;
		const SpriteGraphics gfx_left_bar_;
		const SpriteGraphics gfx_middle_bar_;
		const SpriteGraphics gfx_right_bar_;
};
