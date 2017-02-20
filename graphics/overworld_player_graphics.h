

// Name
//===================================
//
// OverworldPlayerGraphics
//


#ifndef OVERWORLD_PLAYER_GRAPHICS_H
#define OVERWORLD_PLAYER_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.h"


// CLASS
//===================================

    class OverworldPlayerGraphics : public SpriteGraphics
    {
        public:
            OverworldPlayerGraphics();
            ~OverworldPlayerGraphics();

            void update( Sprite& sprite );
    };


#endif // OVERWORLD_PLAYER_GRAPHICS_H
