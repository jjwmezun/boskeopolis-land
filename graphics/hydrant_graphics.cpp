



// Name
//===================================
//
// HydrantGraphics
//


// DEPENDENCIES
//===================================

    #include "hydrant_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    HydrantGraphics::HydrantGraphics()
    :
        SpriteGraphics ( "sprites/hydrant.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 )
    {};

    HydrantGraphics::~HydrantGraphics() {};

    void HydrantGraphics::update( Sprite& sprite )
    {
    };

