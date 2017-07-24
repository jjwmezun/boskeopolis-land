#pragma once

class Health;

#include "sprite_graphics.hpp"
#include "unit.hpp"

class InventoryLevelHealth
{
	public:
		InventoryLevelHealth( int y );
		void update( const Health& health );
		void render();

	private:
		static constexpr int X = Unit::MiniBlocksToPixels( 11 );

		sdl2::SDLRect dest_;
		const SpriteGraphics gfx_;
		const SpriteGraphics empty_gfx_;
		int hearts_shown_;
};