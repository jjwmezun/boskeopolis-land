#pragma once

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class RegularGuardSprite final : public Sprite
{
	public:
		RegularGuardSprite( int x, int y, Direction::Horizontal direction );
		~RegularGuardSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void deathAction( LevelState& level_state ) override;

	private:
		TimerRepeatT2<> animation_timer_;
};
