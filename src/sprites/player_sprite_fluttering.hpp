


// Name
//===================================
//
// PlayerSpriteFluttering
//

#ifndef PLAYER_SPRITE_FLUTTERING_H
#define PLAYER_SPRITE_FLUTTERING_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    
    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class PlayerSpriteFluttering : public Sprite
    {
        public:
            PlayerSpriteFluttering( int x, int y );
            ~PlayerSpriteFluttering();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
    };


#endif // PLAYER_SPRITE_FLUTTERING_H
