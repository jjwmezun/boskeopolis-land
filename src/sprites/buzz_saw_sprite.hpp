


// Name
//===================================
//
// BuzzSawSprite
//

#ifndef BUZZ_SAW_SPRITE_H
#define BUZZ_SAW_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class BuzzSawSprite : public Sprite
    {
        public:
            BuzzSawSprite( int x, int y );
            ~BuzzSawSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
    };


#endif // BUZZ_SAW_SPRITE_H
