#pragma once

class Block;

#include "sprite.hpp"
#include "timers/timer_simple.hpp"

class PlatformSprite final : public Sprite
{
    public:
        PlatformSprite( int x, int y, Direction::Simple direction, int move_space_before_change = 128000 );
        ~PlatformSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};
