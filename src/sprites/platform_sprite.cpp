#include "collision.hpp"
#include "platform_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_component_right_and_left.hpp"
#include "sprite_component_up_and_down.hpp"

PlatformSprite::PlatformSprite( int x, int y, Direction::Simple direction, int move_space_before_change )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/platform.png" ), x, y, 32, 8, {}, 800, 800, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
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

void PlatformSprite::customUpdate( LevelState& level_state )
{
};

void PlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( their_collision.collideBottom() )
    {
        them.collideStopYBottom( their_collision.overlapYBottom() );
        them.addToX( vx_ );
        them.addToY( vy_ );
    }
};
