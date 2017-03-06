





// Name
//===================================
//
// BadAppleGraphics
//


// DEPENDENCIES
//===================================

    #include "bad_apple_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BadAppleGraphics::BadAppleGraphics()
    :
        SpriteGraphics ( "sprites/bad_apple.png" ),
        animation_frame_ ( { 0, 1, 0, true } )
    {};

    BadAppleGraphics::~BadAppleGraphics() {};

    void BadAppleGraphics::update( Sprite& sprite )
    {
        if ( sprite.directionX() == Direction::Horizontal::RIGHT )
        {
            flip_x_ = true;
        }
        else
        {
            flip_x_ = false;
        }

        switch ( animation_frame_() )
        {
            case ( 1 ):
                current_frame_x_ = 16;
            break;
            case ( 0 ):
                current_frame_x_ = 0;
            break;
        }

        if ( animation_timer_.hit() )
        {
            ++animation_frame_;
        }

        animation_timer_.update();
    };

