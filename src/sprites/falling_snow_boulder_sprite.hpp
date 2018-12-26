#pragma once

#include "sprite.hpp"

class FallingSnowBoulderSprite : public Sprite
{
	public:
		FallingSnowBoulderSprite( int x, int y );
		~FallingSnowBoulderSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		int timer_;
};
