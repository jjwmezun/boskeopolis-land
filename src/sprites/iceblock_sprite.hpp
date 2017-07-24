


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
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class IceBlockSprite : public Sprite
    {
        public:
            IceBlockSprite( int x, int y );
            ~IceBlockSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
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