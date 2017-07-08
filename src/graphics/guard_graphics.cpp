





// Name
//===================================
//
// GuardGraphics
//


// DEPENDENCIES
//===================================

    #include "guard_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    GuardGraphics::GuardGraphics()
    :
        SpriteGraphics ( "sprites/guard.png", 0, 0, false, false, 0, false, 0, 0, 0, 0, 16 ),
        animation_frame_ ( { 0, 1, 0, true } )
    {};

    GuardGraphics::~GuardGraphics() {};

    void GuardGraphics::update( Sprite& sprite )
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

