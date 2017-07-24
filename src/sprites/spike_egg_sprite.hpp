



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
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class SpikeEggSprite : public Sprite
    {
        public:
            SpikeEggSprite( int x, int y );
            ~SpikeEggSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
            void reset();
    };


#endif // SPIKE_EGG_SPRITE_H