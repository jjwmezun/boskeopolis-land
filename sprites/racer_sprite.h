



// Name
//===================================
//
// RacerSprite
//

#ifndef RACER_SPRITE_H
#define RACER_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "player_sprite.h"


// CLASS
//===================================

    class RacerSprite : public PlayerSprite
    {
        public:
            static const bool DEBUG = false;
            RacerSprite( int x, int y );
            ~RacerSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
    };


#endif // RACER_SPRITE_H
