#pragma once

class Health;

#include "sprite_graphics.hpp"

class InventoryLevelHealth
{
	public:
		InventoryLevelHealth( int y );
		void update( const Health& health );
		void render();

	private:
		sdl2::SDLRect dest_;
		const SpriteGraphics gfx_;
		int hearts_shown_;
};