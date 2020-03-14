

// Name
//===================================
//
// SpriteComponentCircle
//

#ifndef SPRITE_COMPONENT_CIRCLE_H
#define SPRITE_COMPONENT_CIRCLE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.hpp"
    #include "sprite_component.hpp"


// CLASS
//===================================

    class SpriteComponentCircle final : public SpriteComponent
    {

        public:
            SpriteComponentCircle( int radius = 50, int speed = 50, bool before_update = false );
            ~SpriteComponentCircle();

            void update( Sprite& sprite, SpriteGraphics& graphics );

        private:
            static const int ANGLE_SPEED_CONVERSION = 1000;
            const int radius_;
            const int speed_;
            int angle_;

            double angle() const;
            int radius() const;
    };

#endif // SPRITE_COMPONENT_CIRCLE_H
