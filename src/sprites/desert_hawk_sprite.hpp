#pragma once

#include "sprite.hpp"

class DesertHawkSprite : public Sprite
{
	public:
		DesertHawkSprite( int x, int y, Direction::Horizontal direction );
		~DesertHawkSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int timer_ = 0;
		int frame_ = 0;
};
