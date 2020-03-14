



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

    class SpikeEggSprite final : public Sprite
    {
        public:
            SpikeEggSprite( int x, int y );
            ~SpikeEggSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
            void reset();
    };


#endif // SPIKE_EGG_SPRITE_H
