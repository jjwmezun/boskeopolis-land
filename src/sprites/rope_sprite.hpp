#pragma once

class Block;

#include "sprite.hpp"
#include "sprite_component_right_and_left.hpp"

class RopeSprite final : public Sprite
{
    public:
        RopeSprite( int x, int y, int height = 18, int distance = 14, int speed = 1200 );
        ~RopeSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
        void reset();

    private:
        SpriteComponentRightAndLeft action_;
        bool on_;
};