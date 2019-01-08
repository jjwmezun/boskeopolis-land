#pragma once

#include "sprite.hpp"

class TruckPlatformSprite : public Sprite
{
	public:
		TruckPlatformSprite( int x, int y );
		~TruckPlatformSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;
		void render( Camera& camera, bool priority = false ) override;
};
