




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

    #include "counter.h"
    #include "sprite_graphics.h"


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
