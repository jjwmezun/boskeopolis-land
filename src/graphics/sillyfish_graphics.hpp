



// Name
//===================================
//
// SillyfishGraphics
//


#ifndef SILLYFISH_GRAPHICS_H
#define SILLYFISH_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.hpp"
    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class SillyfishGraphics : public SpriteGraphics
    {
        public:
            SillyfishGraphics();
            ~SillyfishGraphics();

            void update( Sprite& sprite );


        private:
            const int NUM_O_FRAMES = 2;
            Counter animation_frame_;
    };


#endif // SILLYFISH_GRAPHICS_H
