


// Name
//===================================
//
// AnimatedGraphics
//

#ifndef ANIMATED_GRAPHICS_H
#define ANIMATED_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "counter.h"
    #include "sprite_graphics.h"
    #include <vector>
    #include <utility>


// CLASS
//===================================

    class AnimatedGraphics : public SpriteGraphics
    {
        public:
            AnimatedGraphics
            (
                Graphics::SpriteSheet texture,
                std::vector<std::pair<int, int>> frames,
                bool flip_x = false,
                bool flip_y = false,
                double rotation = 0,
                bool priority = false,
                int x_adjustment = 0,
                int y_adjustment = 0,
                int w_adjustment = 0,
                int h_adjustment = 0
            );
            ~AnimatedGraphics();
            void update();

        private:
            std::vector<std::pair<int, int>> frames_;
            Counter current_frame_index_;

            int getXFrameOfPairIndex( int index ) const;
            int getYFrameOfPairIndex( int index ) const;
            void setFrames();
    };

#endif // ANIMATED_GRAPHICS_H
