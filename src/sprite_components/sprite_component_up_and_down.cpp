


// Name
//===================================
//
// SpriteComponentUpAndDown
//


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "sprite_component_up_and_down.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteComponentUpAndDown::SpriteComponentUpAndDown( int move_space_before_change, bool before_update )
    :
        SpriteComponent ( before_update ),
        move_space_before_change_ ( Unit::BlocksToSubPixels( move_space_before_change ) ),
        direction_ ( DEFAULT_DIRECTION )
    {};

    SpriteComponentUpAndDown::~SpriteComponentUpAndDown() {};

    void SpriteComponentUpAndDown::update( Sprite& sprite, SpriteGraphics& graphics )
    {
        switch( direction_ )
        {
            case ( Direction::Vertical::UP ):
                sprite.moveUp();

                if ( sprite.bottomSubPixels() < sprite.originalYSubPixels() - move_space_before_change_ )
                {
                    sprite.stopY();
                    direction_ = Direction::Vertical::DOWN;
                }
            break;
            case ( Direction::Vertical::DOWN ):
                sprite.moveDown();

                if ( sprite.topSubPixels() > sprite.originalYSubPixels() )
                {
                    sprite.stopY();
                    direction_ = Direction::Vertical::UP;
                }
            break;
        }
    };

    void SpriteComponentUpAndDown::reset()
    {
        direction_ = DEFAULT_DIRECTION;
    };
