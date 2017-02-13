


// Name
//===================================
//
// SpriteComponent
//

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;
    class SpriteGraphics;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class SpriteComponent
    {

        public:
            SpriteComponent( bool before_update = false );
            virtual ~SpriteComponent();

            virtual void update( Sprite& sprite, SpriteGraphics& graphics ) = 0;
            virtual void reset();

            bool beforeUpdate() const;

        protected:
            bool before_update_;
    };

#endif // SPRITE_COMPONENT_H
