#pragma once

#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class FallingSpikeSprite final : public Sprite
{
	public:
		FallingSpikeSprite( int x, int y );
		~FallingSpikeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority = false ) override;
		void renderSuperPriority( Camera& camera ) override;
        void reset() override;

    private:
        static constexpr int NUMBER_OF_FRAMES = 13;
        static constexpr int FRAMES[ NUMBER_OF_FRAMES ] = { 0, 1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1, 0 };
        enum class State
        {
            STATIC,
            SHAKING,
            FALLING
        };
        State state_;
        TimerRepeatT2<4> timer_;
        int frame_;
};
