#pragma once

#include "sprite.hpp"

class BarrelOGorillasSprite : public Sprite
{
	public:
		BarrelOGorillasSprite( int x, int y );
		~BarrelOGorillasSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		sdl2::SDLRect top_hit_box_;
		sdl2::SDLRect right_hit_box_;
		int spawn_timer_;
		int spawn_limit_;
};
