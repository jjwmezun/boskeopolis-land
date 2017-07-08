

// Name
//===================================
//
// HydrantGraphics
//


#ifndef HYDRANT_GRAPHICS_H
#define HYDRANT_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.h"


// CLASS
//===================================

    class HydrantGraphics : public SpriteGraphics
    {
        public:
            HydrantGraphics();
            ~HydrantGraphics();

            void update( Sprite& sprite );
    };


#endif // HYDRANT_GRAPHICS_H
