#pragma once

class Block;

#include "sprite.hpp"

class SpringSprite : public Sprite
{
	public:
		SpringSprite( int x, int y );
		~SpringSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		
	private:
		static constexpr int STRENGTH = 24;
};