#pragma once

#include "counter_flip.hpp"
#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class StraightGhostSprite final : public Sprite
{
	public:
		StraightGhostSprite( int x, int y );
		~StraightGhostSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void reset() override;

    private:
        static constexpr int NUMBER_OF_WALK_FRAMES = 4;
        static constexpr int WALK_FRAMES[ NUMBER_OF_WALK_FRAMES ] =
        {
            0, 16, 0, 32
        };
		TimerRepeatT2<8> animation_timer_;
		CounterFlip<NUMBER_OF_WALK_FRAMES - 1> walk_counter_;
        char scratch_[3000];
};
