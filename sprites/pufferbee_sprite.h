




// Name
//===================================
//
// PufferbeeSprite
//

#ifndef PUFFERBEE_SPRITE_H
#define PUFFERBEE_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class PufferbeeSprite : public Sprite
    {
        public:
            PufferbeeSprite( int x, int y, std::unique_ptr<SpriteComponent> component = nullptr );
            ~PufferbeeSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            Direction::Rotation randomDirection() const;
    };


#endif // PUFFERBEE_SPRITE_H
