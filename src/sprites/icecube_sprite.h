



// Name
//===================================
//
// IceCubeSprite
//

#ifndef ICECUBE_SPRITE_H
#define ICECUBE_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class IceCubeSprite : public Sprite
    {
        public:
            IceCubeSprite( int x, int y, Direction::Horizontal direction = Direction::Horizontal::RIGHT, bool start_moving = false );
            ~IceCubeSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
            void reset();

        private:
            bool start_moving_;
            bool start_moving_orig_;
            bool changed_;
    };


#endif // ICECUBE_SPRITE_H
