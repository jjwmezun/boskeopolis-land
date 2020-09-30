#pragma once

#include "sprite.hpp"

class Block;    
class Sprite;

class PlayerSpriteFluttering final : public Sprite
{
    public:
        PlayerSpriteFluttering( int x, int y );
        ~PlayerSpriteFluttering();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};