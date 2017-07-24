


// Name
//===================================
//
// FlutteringSpriteMovement
//

#ifndef FLUTTERING_SPRITE_MOVEMENT_H
#define FLUTTERING_SPRITE_MOVEMENT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "grounded_sprite_movement.hpp"


// CLASS
//===================================

    class FlutteringSpriteMovement : public GroundedSpriteMovement
    {
        public:
            FlutteringSpriteMovement();

            void jump( Sprite& sprite ) const;
            void position( Sprite& sprite ) const;
    };

#endif // FLUTTERING_SPRITE_MOVEMENT_H

