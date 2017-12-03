#pragma once

#include "sprite.hpp"

class SnowballSprite : public Sprite
{
	public:
		SnowballSprite( int x, int y, Direction::Horizontal dir );
		~SnowballSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
		
	private:
		const int fall_speed_;
		int counter_;
};