#pragma once

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class Block;

class PenguinSprite : public Sprite
{
    public:
        PenguinSprite( int x, int y );
        ~PenguinSprite();
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

    private:
        bool turning_;
        TimerSimpleT<48, false> delay_;
};
