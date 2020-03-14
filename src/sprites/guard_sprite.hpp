#pragma once

class Block;

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"
#include "timer_simple_t.hpp"

class GuardSprite final : public Sprite
{
    public:
        GuardSprite( int x, int y );
        ~GuardSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

    private:
        void updateGraphics();

        bool stunned_;
        int distance_limit_y_;
		CounterT<1, 0, 0, true> animation_frame_;
        TimerSimpleT<8, false> stun_timer_;
		TimerRepeatT<16> animation_timer_;
};
