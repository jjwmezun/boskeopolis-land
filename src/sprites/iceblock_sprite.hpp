#pragma once

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class Block;

class IceBlockSprite : public Sprite
{
    public:
        IceBlockSprite( int x, int y );
        ~IceBlockSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
        void reset();

    private:
        enum class CrackState
        {
            NORMAL,
            CRACKING_FEET,
            CRACKING_HEAD,
            CRACKING_GROWING,
            SHATTERED,
            GONE_GROWING
        };
        CrackState crack_state_;
        TimerSimpleT<16, false> delay_;
        TimerSimpleT<64, false> grow_delay_;
};
