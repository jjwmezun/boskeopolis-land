

// Name
//===================================
//
// SpriteMovement
//

#ifndef SPRITE_MOVEMENT_H
#define SPRITE_MOVEMENT_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class SpriteMovement
    {
        public:
            enum class Type
            {
                GROUNDED,
                FLOATING,
                FLUTTERING,
                SWIMMING
            };

            SpriteMovement( Type type = Type::FLOATING );
            Type type() const;
            bool hasType( Type type ) const;

            virtual void moveLeft( Sprite* sprite );
            virtual void moveRight( Sprite* sprite );
            virtual void moveUp( Sprite* sprite );
            virtual void moveDown( Sprite* sprite );
            virtual void jump( Sprite* sprite );
            virtual void bounce( Sprite* sprite, int amount );
            virtual void position( Sprite* sprite );
            virtual void collideStopXLeft( Sprite* sprite, int overlap );
            virtual void collideStopXRight( Sprite* sprite, int overlap );
            virtual void collideStopYBottom( Sprite* sprite, int overlap );
            virtual void collideStopYTop( Sprite* sprite, int overlap );
            virtual void collideStopAny( Sprite* sprite, Collision& collision );

        protected:
            const Type type_;
    };

#endif // SPRITE_MOVEMENT_H

