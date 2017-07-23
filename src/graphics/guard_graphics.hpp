



// Name
//===================================
//
// GuardGraphics
//


#ifndef GUARD_GRAPHICS_H
#define GUARD_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.hpp"
    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class GuardGraphics : public SpriteGraphics
    {
        public:
            GuardGraphics();
            ~GuardGraphics();

            void update( Sprite& sprite );

        private:
            Counter animation_frame_;
    };


#endif // GUARD_GRAPHICS_H
