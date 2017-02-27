



// Name
//===================================
//
// RopeGraphics
//


#ifndef ROPE_GRAPHICS_H
#define ROPE_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.h"


// CLASS
//===================================

    class RopeGraphics : public SpriteGraphics
    {
        public:
            RopeGraphics();
            ~RopeGraphics();

            void update( Sprite& sprite );
            void render( Graphics& graphics, sdl2::SDLRect bound_box, Camera* camera = nullptr, bool priority = false ) const;

        private:
            static constexpr std::pair<int, int> frame_body_ = { 200, 112 };
            static constexpr std::pair<int, int> frame_end_ = { 208, 112 };
    };


#endif // ROPE_GRAPHICS_H
