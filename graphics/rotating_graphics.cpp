

// Name
//===================================
//
// SpriteMovement
//


// DEPENDENCIES
//===================================

    #include "rotating_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    RotatingGraphics::RotatingGraphics
    (
        Graphics::SpriteSheet texture,
        int current_frame_x,
        int current_frame_y,
        bool flip_x,
        bool flip_y,
        double rotation,
        bool priority
    )
    :
        SpriteGraphics( texture, current_frame_x, current_frame_y, flip_x, flip_y, rotation, priority )
    {};

    RotatingGraphics::~RotatingGraphics() {};

    void RotatingGraphics::update()
    {
        rotation_ -= 5;
    };
