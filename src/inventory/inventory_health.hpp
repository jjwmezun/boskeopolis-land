#pragma once

class Sprite;

#include "sprite_graphics.hpp"

class InventoryHealth
{
	public:
		InventoryHealth( int y );
		void update( Sprite& hero );
		void render();


	private:
		sdl2::SDLRect dest_;
		const SpriteGraphics gfx_;
		int hearts_shown_;

};