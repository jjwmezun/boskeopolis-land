


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

    #include "grounded_sprite_movement.h"


// CLASS
//===================================

    class FlutteringSpriteMovement : public GroundedSpriteMovement
    {
        public:
            FlutteringSpriteMovement();

            void jump( Sprite& sprite );
            void position( Sprite& sprite );
    };

#endif // FLUTTERING_SPRITE_MOVEMENT_H

