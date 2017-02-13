

// Name
//===================================
//
// OverworldPlayerSprite
//

#ifndef OVERWORLD_PLAYER_SPRITE_H
#define OVERWORLD_PLAYER_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class OverworldPlayerSprite : public Sprite
    {
        public:
            OverworldPlayerSprite( int x, int y );
            ~OverworldPlayerSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // OVERWORLD_PLAYER_SPRITE_H
