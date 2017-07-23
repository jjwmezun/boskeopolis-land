




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
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class PufferbeeSprite : public Sprite
    {
        public:
            PufferbeeSprite
			(
				int x,
				int y,
				std::unique_ptr<SpriteComponent> component = nullptr
			);
            ~PufferbeeSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            Direction::Rotation randomDirection() const;
    };


#endif // PUFFERBEE_SPRITE_H
