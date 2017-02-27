




// Name
//===================================
//
// PlatformSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "platform_sprite.h"
    #include "sprite_graphics.h"
    #include "sprite_component_right_and_left.h"
    #include "sprite_component_up_and_down.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PlatformSprite::PlatformSprite( int x, int y, Direction::Simple direction, int move_space_before_change )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_PLATFORM ) ), x, y, 32, 8, {}, 800, 800, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
    {
        switch ( Direction::SimpleIsOfWhatType( direction ) )
        {
            case ( Direction::Type::HORIZONTAL ):
                component_.reset( new SpriteComponentRightAndLeft( move_space_before_change ) );
            break;
            case ( Direction::Type::VERTICAL ):
                component_.reset( new SpriteComponentUpAndDown( move_space_before_change ) );
            break;
        }
    };

    PlatformSprite::~PlatformSprite() {};

    void PlatformSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
    };

    void PlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( their_collision.collideBottom() )
        {
            them.collideStopYBottom( their_collision.overlapYBottom() );
            them.addToX( vx_ );
            them.addToY( vy_ );
        }
    };
