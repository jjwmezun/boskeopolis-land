#pragma once

class Block;

#include "sprite.hpp"

class LifesaverSprite : public Sprite
{
	public:
		LifesaverSprite( int x, int y, Direction::Horizontal direction = Direction::Horizontal::RIGHT, bool start_moving = false );
		~LifesaverSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
		void reset();
};