#pragma once

#include "bump_under_block_sprite.hpp"
#include "sprite.hpp"

class LightSwitchSprite : public Sprite
{
	public:
		LightSwitchSprite( int x, int y );
		~LightSwitchSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

    private:
		BumpUnderBlockSprite bump_under_block_component;
};
