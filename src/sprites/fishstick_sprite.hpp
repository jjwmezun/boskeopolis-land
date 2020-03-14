#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class FishstickSprite final : public Sprite
{
	public:
		FishstickSprite( int x, int y );
		~FishstickSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

	private:
		TimerSimpleT<16, true> move_timer_;
		TimerSimpleT<8, false> pause_timer_;
};
