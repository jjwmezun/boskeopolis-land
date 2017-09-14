#pragma once

class Block;

#include "sprite.hpp"

class HieroglyphPusherSprite : public Sprite
{
	public:
		HieroglyphPusherSprite( int x, int y );
		~HieroglyphPusherSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
};