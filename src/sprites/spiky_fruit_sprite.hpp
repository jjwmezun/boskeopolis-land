#pragma once

#include "sprite.hpp"

class Block;

class SpikyFruitSprite : public Sprite
{
	public:
		SpikyFruitSprite( int x, int y );
		~SpikyFruitSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void reset() override;
};