



// Name
//===================================
//
// SpriteComponentRightAndLeft
//


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "sprite_component_right_and_left.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteComponentRightAndLeft::SpriteComponentRightAndLeft( int move_space_before_change, bool before_update )
    :
        SpriteComponent ( before_update ),
        move_space_before_change_ ( Unit::BlocksToSubPixels( move_space_before_change ) ),
        direction_ ( DEFAULT_DIRECTION )
    {};

    SpriteComponentRightAndLeft::~SpriteComponentRightAndLeft() {};

    void SpriteComponentRightAndLeft::update( Sprite& sprite, SpriteGraphics& graphics )
    {
        switch( direction_ )
        {
            case ( Direction::Horizontal::RIGHT ):
                sprite.moveRight();

                if ( sprite.leftSubPixels() > sprite.originalXSubPixels() + move_space_before_change_ )
                {
                    sprite.stopX();
                    direction_ = Direction::Horizontal::LEFT;
                }
            break;
            case ( Direction::Horizontal::LEFT ):
                sprite.moveLeft();

                if ( sprite.rightSubPixels() < sprite.originalXSubPixels() )
                {
                    sprite.stopX();
                    direction_ = Direction::Horizontal::RIGHT;
                }
            break;
        }
    };

    void SpriteComponentRightAndLeft::reset()
    {
        direction_ = DEFAULT_DIRECTION;
    };
