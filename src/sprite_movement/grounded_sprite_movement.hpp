

// Name
//===================================
//
// GroundedSpriteMovement
//

#ifndef GROUNDED_SPRITE_MOVEMENT_H
#define GROUNDED_SPRITE_MOVEMENT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "sprite_movement.hpp"


// CLASS
//===================================

    class GroundedSpriteMovement : public SpriteMovement
    {
        public:
            GroundedSpriteMovement();

            virtual void moveUp( Sprite& sprite );
            virtual void moveDown( Sprite& sprite );
            virtual void jump( Sprite& sprite );
            virtual void bounce( Sprite& sprite, int amount );
            virtual void position( Sprite& sprite );
            void collideStopYBottom( Sprite& sprite, int overlap );
            void collideStopYTop( Sprite& sprite, int overlap );
            void collideStopAny( Sprite& sprite, Collision& collision );
			const Collision testCollision( const Sprite& me, const Object& them ) const;

        protected:
            GroundedSpriteMovement( Type type );

        private:
            void startJump( Sprite& sprite );
            bool onGroundPadding( Sprite& sprite ) const;
            int bounce_height_ = 0;
    };

#endif // GROUNDED_SPRITE_MOVEMENT_H

