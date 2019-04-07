#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class FishstickSprite : public Sprite
{
	public:
		FishstickSprite( int x, int y );
		~FishstickSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

	private:
		TimerSimpleT<16, true> move_timer_;
		TimerSimpleT<8, false> pause_timer_;
};
