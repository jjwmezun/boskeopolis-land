#pragma once

#include "sprite.hpp"

class MoveWaterSprite : public Sprite
{
	public:
		MoveWaterSprite( int x, int y );
		~MoveWaterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Clockwise spin_direction_;
		void readjustGraphics();
};
