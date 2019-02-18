#pragma once

#include "sprite.hpp"

class AntSprite : public Sprite
{
	public:
		AntSprite( int x, int y );
		~AntSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Rotation direction_;
		int slope_timer_;
		int animation_timer_;

		void testSlopeChange( Direction::Rotation next_direction );
};
