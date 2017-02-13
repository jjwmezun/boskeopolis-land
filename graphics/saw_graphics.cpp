




// Name
//===================================
//
// SawGraphics
//


// DEPENDENCIES
//===================================

    #include "saw_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SawGraphics::SawGraphics()
    :
        SpriteGraphics ( Graphics::SpriteSheet::LVSPRITES_SAW, 0, 0, false, false, 0, true, 0, 0, 16, 0 )
    {};

    SawGraphics::~SawGraphics() {};

    void SawGraphics::update( Sprite& sprite )
    {
    };

