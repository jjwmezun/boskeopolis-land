



// Name
//===================================
//
// SpriteComponentUpAndDown
//

#ifndef SPRITE_COMPONENT_UP_AND_DOWN_H
#define SPRITE_COMPONENT_UP_AND_DOWN_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.hpp"
    #include "sprite_component.hpp"


// CLASS
//===================================

    class SpriteComponentUpAndDown : public SpriteComponent
    {

        public:
            SpriteComponentUpAndDown( int move_space_before_change = 8, bool before_update = false );
            ~SpriteComponentUpAndDown();

            void update( Sprite& sprite, SpriteGraphics& graphics );
            void reset();

        private:
            static const Direction::Vertical DEFAULT_DIRECTION = Direction::Vertical::UP;
            const int move_space_before_change_;
            Direction::Vertical direction_;
    };

#endif // SPRITE_COMPONENT_UP_AND_DOWN_H
