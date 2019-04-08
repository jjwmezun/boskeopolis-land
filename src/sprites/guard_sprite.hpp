#pragma once

class Block;

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"
#include "timer_simple_t.hpp"

class GuardSprite : public Sprite
{
    public:
        GuardSprite( int x, int y );
        ~GuardSprite();
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

    private:
        void updateGraphics();

        bool stunned_;
        int distance_limit_y_;
		CounterT<1, 0, 0, true> animation_frame_;
        TimerSimpleT<8, false> stun_timer_;
		TimerRepeatT<16> animation_timer_;
};
