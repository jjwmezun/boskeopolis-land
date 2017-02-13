


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
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // PLAYER_SPRITE_FLUTTERING_H
