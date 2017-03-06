



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
            void render( Graphics& graphics, const sdl2::SDLRect& bound_box, Camera* camera = nullptr, bool priority = false ) const override;

        private:
            static constexpr std::pair<int, int> frame_body_ = { 112, 16 };
            static constexpr std::pair<int, int> frame_end_ = { 120, 16 };
    };


#endif // ROPE_GRAPHICS_H
