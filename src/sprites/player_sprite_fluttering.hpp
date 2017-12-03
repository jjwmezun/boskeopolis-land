


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
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
    };


#endif // PLAYER_SPRITE_FLUTTERING_H
