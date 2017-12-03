#pragma once

class Block;

#include "sprite.hpp"

class BatSprite : public Sprite
{
	public:
		BatSprite( int x, int y );
		~BatSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		
		private:
			bool awake_;
};