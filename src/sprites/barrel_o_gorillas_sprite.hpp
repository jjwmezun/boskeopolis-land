#pragma once

#include "sprite.hpp"

class BarrelOGorillasSprite : public Sprite
{
	public:
		BarrelOGorillasSprite( int x, int y );
		~BarrelOGorillasSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		sdl2::SDLRect top_hit_box_;
		sdl2::SDLRect right_hit_box_;
		int spawn_timer_;
		int spawn_limit_;
};
