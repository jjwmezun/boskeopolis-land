



// Name
//===================================
//
// BadAppleGraphics
//


#ifndef BAD_APPLE_GRAPHICS_H
#define BAD_APPLE_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.hpp"
    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class BadAppleGraphics : public SpriteGraphics
    {
        public:
            BadAppleGraphics();
            ~BadAppleGraphics();

            void update( Sprite& sprite );

        private:
            Counter animation_frame_;
    };


#endif // BAD_APPLE_GRAPHICS_H
