





// Name
//===================================
//
// RopeGraphics
//


// DEPENDENCIES
//===================================

    #include "rope_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================

    constexpr std::pair<int, int> RopeGraphics::frame_body_;
    constexpr std::pair<int, int> RopeGraphics::frame_end_;


// METHODS
//===================================

    RopeGraphics::RopeGraphics()
    :
        SpriteGraphics ( Graphics::SpriteSheet::LVTILESET_CITY, 200, 112 )
    {};

    RopeGraphics::~RopeGraphics() {};

    void RopeGraphics::update( Sprite& sprite )
    {
    };

    void RopeGraphics::render( Graphics& graphics, sdl2::SDLRect bound_box, Camera* camera, bool priority )
    {
        const int LAST_BLOCK = bound_box.h - 8;

        current_frame_x_ = std::get<X_INDEX> ( frame_body_ );
        current_frame_y_ = std::get<Y_INDEX> ( frame_body_ );

        for ( int i = 0; i < LAST_BLOCK; i += 8 )
        {
            const sdl2::SDLRect dest =
            {
                bound_box.x,
                bound_box.y + i,
                8,
                8
            };

            masterRender( graphics, dest, camera, priority );
        }

        current_frame_x_ = std::get<X_INDEX> ( frame_end_ );
        current_frame_y_ = std::get<Y_INDEX> ( frame_end_ );

        const sdl2::SDLRect dest =
        {
            bound_box.x,
            LAST_BLOCK,
            8,
            8
        };

        masterRender( graphics, dest, camera, priority );
    };

