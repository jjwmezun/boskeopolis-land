#pragma once

class Block;

#include "sprite.hpp"
#include "timers/timer_simple.hpp"

class SawSprite : public Sprite
{
    public:
        SawSprite( int x, int y );
        ~SawSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

    private:
        int counter_ = 0;
        bool going_up_ = false;
};