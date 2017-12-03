#pragma once

class Block;

#include "sprite.hpp"

class PipeEelSprite : public Sprite
{
	public:
		PipeEelSprite( int x, int y, Direction::Vertical dir_y );
		~PipeEelSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
};