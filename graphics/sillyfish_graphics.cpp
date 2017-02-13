


// Name
//===================================
//
// SillyfishGraphics
//


// DEPENDENCIES
//===================================

    #include "sillyfish_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SillyfishGraphics::SillyfishGraphics()
    :
        SpriteGraphics ( Graphics::SpriteSheet::LVSPRITES_SILLYFISH ),
        animation_frame_ ( { 0, NUM_O_FRAMES-1, 0, true } )
    {};

    SillyfishGraphics::~SillyfishGraphics() {};

    void SillyfishGraphics::update( Sprite& sprite )
    {
        if ( sprite.directionX() == Direction::Horizontal::RIGHT )
        {
            flip_x_ = true;
        }
        else
        {
            flip_x_ = false;
        }

        switch ( animation_frame_.value() )
        {
            case ( 0 ):
                current_frame_x_ = 0;
            break;
            case ( 1 ):
                current_frame_x_ = 16;
            break;
        }

        if ( animation_timer_.hit() )
        {
            ++animation_frame_;
        }

        animation_timer_.update();

    };

