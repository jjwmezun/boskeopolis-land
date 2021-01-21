#pragma once

#include "sprite.hpp"

class ShooterGuardSprite final : public Sprite
{
	public:
		ShooterGuardSprite( int x, int y );
		~ShooterGuardSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        enum class State
        {
            PACING,
            STOP
        };
        State state_;
        int timer_;
        char scratch_[3000];
};
