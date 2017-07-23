


// Name
//===================================
//
// SawGraphics
//


#ifndef SAW_GRAPHICS_H
#define SAW_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.hpp"


// CLASS
//===================================

    class SawGraphics : public SpriteGraphics
    {
        public:
            SawGraphics();
            ~SawGraphics();

            void update( Sprite& sprite );
    };


#endif // SAW_GRAPHICS_H
