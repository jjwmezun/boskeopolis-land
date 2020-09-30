#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "saw_sprite.hpp"
#include "sprite_graphics.hpp"

SawSprite::SawSprite( int x, int y )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/saw.png", 0, 0, false, false, 0, 0, 0, 16, 0 ), x, y, 64, 256, {}, 1500, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, true, true )
{
	layer_ = Unit::Layer::SPRITES_2;
};

SawSprite::~SawSprite() {};

void SawSprite::customUpdate( LevelState& level_state )
{
    moveRight();

    Camera& camera = level_state.camera();
    if ( level_state.camera().x() < Unit::SubPixelsToPixels( hit_box_.x ) + 4 )
        camera.moveRight( Unit::SubPixelsToPixels( hit_box_.x ) + 4 - camera.x() );

    ++counter_;

    if ( counter_ % 16 == 1 )
        going_up_ = !going_up_;

    if ( going_up_ )
        hit_box_.y-=1000;
    else
        hit_box_.y+=1000;
};

void SawSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( their_collision.collideAny() && them.hasType( SpriteType::HERO ) )
    {
        them.killNoAnimation();
    }
};
