


// Name
//===================================
//
// HandgunSprite
//

#ifndef HANDGUN_SPRITE_H
#define HANDGUN_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class HandgunSprite : public Sprite
    {
        public:
            HandgunSprite( int x, int y );
            ~HandgunSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
            void reset();

        private:
            static const int SHOOT_CHANCE_PERCENT = 2;
            static const int RANGE_INTERVAL = Unit::PixelsToSubPixels( 8 );
            bool shooting_ = false;
            bool can_shoot_ = true;

            void testIfItShouldShoot();
    };


#endif // HANDGUN_SPRITE_H
