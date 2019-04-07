#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class GuardSprite : public Sprite
{
    public:
        GuardSprite( int x, int y );
        ~GuardSprite();
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

    private:
        bool stunned_;
        int distance_limit_y_;
        TimerSimpleT<8, false> stun_timer_;
};
