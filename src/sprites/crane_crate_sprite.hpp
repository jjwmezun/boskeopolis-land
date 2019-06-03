#pragma once

#include "circle_movement.hpp"
#include "sprite.hpp"

class CraneCrateSprite : public Sprite
{
	public:
		CraneCrateSprite( int x, int y );
		~CraneCrateSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		CircleMovement circle_;
		int prev_x_;
		int prev_y_;
};
