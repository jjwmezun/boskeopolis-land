#include "belligerant_citizen_sprite.hpp"
#include "level_state.hpp"
#include "shmup_bullet_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

BelligerantCitizenSprite::BelligerantCitizenSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn-skirt.png" ), x + 8, y + 30, 16, 0, { SpriteType::BOPPABLE }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 1 ),
    is_up_ ( false )
{};

BelligerantCitizenSprite::~BelligerantCitizenSprite() {};

void BelligerantCitizenSprite::customUpdate( LevelState& level_state )
{
    if ( !is_up_ )
    {
        hit_box_.h += Unit::PixelsToSubPixels( 1 );
        hit_box_.y -= Unit::PixelsToSubPixels( 1 );
        if ( hit_box_.h == Unit::PixelsToSubPixels( 18 ) )
        {
            is_up_ = true;
        }
    }
    ++timer_;
};

void BelligerantCitizenSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( is_up_ && timer_ >= 32 && them.hasType( SpriteType::HERO ) )
    {
        timer_ = 0;
        const int target_x = them.centerXSubPixels() + Unit::PixelsToSubPixels(( them.acceleration_x_ > 0 ) ? 32 : ( ( them.acceleration_x_ < 0 ) ? -32 : 0 ));
        const int target_y = them.centerYSubPixels() + Unit::PixelsToSubPixels(( them.acceleration_y_ > 0 ) ? 32 : ( ( them.acceleration_y_ < 0 ) ? -32 : 0 ));
        const double dx = ( double )( target_x - centerXSubPixels() );
        const double dy = ( double )( target_y - centerYSubPixels() );
        level_state.sprites().spawn( std::unique_ptr<ShmupBulletSprite> ( new ShmupBulletSprite( centerXPixels(), centerYPixels(), dy, dx ) ) );
    }
};

void BelligerantCitizenSprite::reset()
{
    hit_box_ = original_hit_box_;
    is_up_ = false;
    timer_ = 1;
};