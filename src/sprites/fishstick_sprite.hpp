#pragma once

class Block;

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class FishstickSprite final : public Sprite
{
	public:
		FishstickSprite( int x, int y, Direction::Horizontal direction );
		~FishstickSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

	private:
		TimerRepeatT2<> animation_timer_;
		int animation_frame_;
};
