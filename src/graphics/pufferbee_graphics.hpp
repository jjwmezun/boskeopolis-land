




// Name
//===================================
//
// PufferbeeGraphics
//


#ifndef PUFFERBEE_GRAPHICS_H
#define PUFFERBEE_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.hpp"
    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class PufferbeeGraphics : public SpriteGraphics
    {
        public:
            PufferbeeGraphics();
            ~PufferbeeGraphics();

            void update( Sprite& sprite );

        private:
            Counter animation_frame_;
    };


#endif // PUFFERBEE_GRAPHICS_H
