#pragma once

#include "sprite.hpp"

class CrabSprite : public Sprite
{
	public:
		CrabSprite( int x, int y );
		~CrabSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Horizontal next_direction_;
		int timer_;
		int animation_timer_;
};
