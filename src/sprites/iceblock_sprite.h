


// Name
//===================================
//
// IceBlockSprite
//

#ifndef ICEBLOCK_SPRITE_H
#define ICEBLOCK_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class IceBlockSprite : public Sprite
    {
        public:
            IceBlockSprite( int x, int y );
            ~IceBlockSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
            void reset();

        private:
            enum class CrackState
            {
                NORMAL,
                CRACKING_FEET,
                CRACKING_HEAD,
                CRACKING_GROWING,
                SHATTERED,
                GONE_GROWING
            };
            CrackState crack_state_;
            TimerSimple delay_;
            TimerSimple grow_delay_;
    };


#endif // ICEBLOCK_SPRITE_H
