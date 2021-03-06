#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"
#include "timer_simple_t.hpp"

class Block;

class PenguinSprite final : public Sprite
{
    public:
        PenguinSprite( int x, int y );
        ~PenguinSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

    private:
        void updateGraphics();

		static constexpr int NUM_O_FRAMES = 4;

        bool turning_;
		CounterT<NUM_O_FRAMES - 1, 0, 0, true> animation_frame_;
        TimerSimpleT<48, false> delay_;
		TimerRepeatT<> animation_timer_;
};
