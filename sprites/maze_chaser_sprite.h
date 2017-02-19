




// Name
//===================================
//
// MazeChaserSprite
//

#ifndef MAZE_CHASER_SPRITE_H
#define MAZE_CHASER_SPRITE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class MazeChaserSprite : public Sprite
    {
        public:
            MazeChaserSprite( int x, int y );
            ~MazeChaserSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // MAZE_CHASER_SPRITE_H
