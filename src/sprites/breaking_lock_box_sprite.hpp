#pragma once

#include "sprite.hpp"

class BreakingLockBoxSprite : public Sprite
{
	public:
		BreakingLockBoxSprite( int x, int y, int type );
		~BreakingLockBoxSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
};
