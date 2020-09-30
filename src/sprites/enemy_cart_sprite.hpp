#pragma once

#include "sprite.hpp"

class Block;

class EnemyCartSprite final : public Sprite
{
    public:
        EnemyCartSprite( int x, int y );
        ~EnemyCartSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};