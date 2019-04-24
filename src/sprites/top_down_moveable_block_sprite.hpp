#pragma once

#include "sprite.hpp"

class TopDownMoveableBlockSprite : public Sprite
{
	public:
		TopDownMoveableBlockSprite( int x, int y );
		~TopDownMoveableBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
};
