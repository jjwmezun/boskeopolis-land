#pragma once

#include "sprite.hpp"

class PolloSprite : public Sprite
{
	public:
		PolloSprite( int x, int y, Direction::Horizontal dir_x );
		~PolloSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		
	private:
		Direction::Horizontal switch_from_;
};