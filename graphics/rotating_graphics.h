

// Name
//===================================
//
// RotatingGraphics
//

#ifndef ROTATING_GRAPHICS_H
#define ROTATING_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "sprite_graphics.h"


// CLASS
//===================================

    class RotatingGraphics : public SpriteGraphics
    {
        public:
            RotatingGraphics
            (
                Graphics::SpriteSheet texture = Graphics::SpriteSheet::LIMIT,
                int current_frame_x = 0,
                int current_frame_y = 0,
                bool flip_x = false,
                bool flip_y = false,
                double rotation = 0,
                bool priority = false
            );
            ~RotatingGraphics();
            void update();
    };


#endif // ROTATING_GRAPHICS_H

