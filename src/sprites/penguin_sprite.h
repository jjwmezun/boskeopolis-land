

// Name
//===================================
//
// PenguinSprite
//

#ifndef PENGUIN_SPRITE_H
#define PENGUIN_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class PenguinSprite : public Sprite
    {
        public:
            PenguinSprite( int x, int y );
            ~PenguinSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            bool turning_;
            TimerSimple delay_;
    };


#endif // PENGUIN_SPRITE_H
