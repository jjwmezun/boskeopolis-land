#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class FrogSprite final : public Sprite
{
	public:
		FrogSprite( int x, int y );
		~FrogSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;
		void deathAction( LevelState& level_state ) override;

    private:
		void setTongueX();
		void setTongueY();

        enum class FrogState
        {
            STATIONARY,
			JUMPING,
			LICKING,
			LICKING_BACK
        };
        TimerRandom<40, 10> jump_timer_;
        sdl2::SDLRect tongue_;
        FrogState state_;
        int tongue_speed_;
};
