

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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "sprite_component_right_and_left.h"


// CLASS
//===================================

    class RopeSprite : public Sprite
    {
        public:
            RopeSprite( int x, int y, int height = 18, int distance = 14, int speed = 1200 );
            ~RopeSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
            void reset();

        private:
            SpriteComponentRightAndLeft action_;
            bool on_;
    };


#endif // ROPE_SPRITE_H
