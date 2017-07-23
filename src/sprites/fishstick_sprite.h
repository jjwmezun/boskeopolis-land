#pragma once

class Block;


#include "sprite.h"

class FishstickSprite : public Sprite
{
	public:
		FishstickSprite( int x, int y );
		~FishstickSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	private:
		TimerSimple move_timer_;
		TimerSimple pause_timer_;
};