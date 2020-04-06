#include "electric_wall_sprite.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

ElectricWallSprite::ElectricWallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/electric-wall.png", 0, 0, false, false, 0.0, false, -1, 0, 2, 0 ), x + 6, y, 2, 96, {}, 500, 500, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false )
{};

ElectricWallSprite::~ElectricWallSprite() {};

void ElectricWallSprite::customUpdate( LevelState& level_state )
{
    if ( level_state.events().isSwitchOn() )
    {
        if ( hit_box_.h > 0 )
        {
            hit_box_.h -= 500;
        }
        else
        {
            hit_box_.h = 0;
        }
    }
    else
    {
        if ( hit_box_.h < Unit::PixelsToSubPixels( 96 ) )
        {
            hit_box_.h += 500;
        }
        else
        {
            hit_box_.h = Unit::PixelsToSubPixels( 96 );
        }
    }
};

void ElectricWallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
    {
        level_state.health().hurt();
        if ( their_collision.collideTop() )
        {
            them.hit_box_.x = hit_box_.x - them.hit_box_.w - 1000;
        }
        else
        {
            them.collideStopAny( their_collision );
        }
    }
};
