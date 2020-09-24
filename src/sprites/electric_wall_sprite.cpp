#include "camera.hpp"
#include "electric_wall_sprite.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "render.hpp"
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
    graphics_->current_frame_x_ = mezun::randInt( 3, 0 ) * 4;
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

void ElectricWallSprite::render( Camera& camera, bool priority )
{
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
    const sdl2::SDLRect src1 = { 16, 6, 7, 8 };
    const sdl2::SDLRect dest1
    { 
        camera.relativeX( xPixels() - 3 ),
        camera.relativeY( yPixels() + 90 ),
        7,
        8
    };
    Render::renderObject
    (
        graphics_->texture_,
        src1,
        dest1
    );
    const sdl2::SDLRect src2 = { 17, 0, 5, 6 };
    const sdl2::SDLRect dest2
    { 
        camera.relativeX( xPixels() - 2 ),
        camera.relativeY( yPixels() + 8 ),
        5,
        6
    };
    Render::renderObject
    (
        graphics_->texture_,
        src2,
        dest2
    );
};
