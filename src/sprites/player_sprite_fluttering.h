


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
    class Input;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class PlayerSpriteFluttering : public Sprite
    {
        public:
            PlayerSpriteFluttering( int x, int y );
            ~PlayerSpriteFluttering();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
    };


#endif // PLAYER_SPRITE_FLUTTERING_H
