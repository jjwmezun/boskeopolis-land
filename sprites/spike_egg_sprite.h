



// Name
//===================================
//
// SpikeEggSprite
//

#ifndef SPIKE_EGG_SPRITE_H
#define SPIKE_EGG_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class SpikeEggSprite : public Sprite
    {
        public:
            SpikeEggSprite( int x, int y );
            ~SpikeEggSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
            void reset();
    };


#endif // SPIKE_EGG_SPRITE_H
