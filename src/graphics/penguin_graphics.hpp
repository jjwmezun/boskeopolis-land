


// Name
//===================================
//
// PenguinGraphics
//


#ifndef PENGUIN_GRAPHICS_H
#define PENGUIN_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.hpp"
    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class PenguinGraphics : public SpriteGraphics
    {
        public:
            PenguinGraphics();
            ~PenguinGraphics();

            void update( Sprite& sprite );


        private:
            static constexpr int NUM_O_FRAMES = 4;
            Counter animation_frame_;
    };


#endif // PENGUIN_GRAPHICS_H
