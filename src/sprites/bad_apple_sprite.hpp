#pragma once

class Block;

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class BadAppleSprite final : public Sprite
{
	public:
		BadAppleSprite( int x, int y, Direction::Horizontal dir_x );
		~BadAppleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		TimerRepeatT2<> animation_timer_;
};
