#pragma once

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class ShooterGuardSprite final : public Sprite
{
	public:
		ShooterGuardSprite( int x, int y );
		~ShooterGuardSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void deathAction( LevelState& level_state ) override;

    private:
        enum class State
        {
            PACING,
            STOP
        };
        State state_;
        TimerRepeatT2<30, 0, 30> shoot_timer_;
		TimerRepeatT2<> animation_timer_;
};
