#pragma once

class Block;

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class AlarmClockSprite final : public Sprite
{
	public:
		AlarmClockSprite( int x, int y, Direction::Horizontal dir_x );
		~AlarmClockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
};
