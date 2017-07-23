




// Name
//===================================
//
// SpriteComponentRightAndLeft
//

#ifndef SPRITE_COMPONENT_RIGHT_AND_LEFT_H
#define SPRITE_COMPONENT_RIGHT_AND_LEFT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.hpp"
    #include "sprite_component.hpp"


// CLASS
//===================================

    class SpriteComponentRightAndLeft : public SpriteComponent
    {

        public:
            SpriteComponentRightAndLeft( int move_space_before_change = 8, bool before_update = false );
            ~SpriteComponentRightAndLeft();

            void update( Sprite& sprite, SpriteGraphics& graphics );
            void reset();

        private:
            static const Direction::Horizontal DEFAULT_DIRECTION = Direction::Horizontal::RIGHT;
            const int move_space_before_change_;
            Direction::Horizontal direction_;
    };

#endif // SPRITE_COMPONENT_RIGHT_AND_LEFT_H
