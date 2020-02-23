

// Name
//===================================
//
// RopeSprite
//

#ifndef ROPE_SPRITE_H
#define ROPE_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "sprite_component_right_and_left.hpp"


// CLASS
//===================================

    class RopeSprite : public Sprite
    {
        public:
            RopeSprite( int x, int y, int height = 18, int distance = 14, int speed = 1200 );
            ~RopeSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
            void reset();

        private:
            SpriteComponentRightAndLeft action_;
            bool on_;
    };


#endif // ROPE_SPRITE_H
