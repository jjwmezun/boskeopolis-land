




// Name
//===================================
//
// SwimmingSpriteMovement
//

#ifndef SWIMMING_SPRITE_MOVEMENT_H
#define SWIMMING_SPRITE_MOVEMENT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "grounded_sprite_movement.h"


// CLASS
//===================================

    class SwimmingSpriteMovement : public GroundedSpriteMovement
    {
        public:
            SwimmingSpriteMovement();

            void jump( Sprite& sprite );
            void position( Sprite& sprite );

        private:
            static const int SWIM_START_SPEED = 5000;
    };

#endif // SWIMMING_SPRITE_MOVEMENT_H
