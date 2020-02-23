#include "bridge_monster_projectile_sprite.hpp"
#include "bridge_monster_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include <memory>

BridgeMonsterSprite::BridgeMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    jump_timer_ (),
    walk_timer_ (),
    shoot_ ( false )
{
    jump_lock_ = true;
};

BridgeMonsterSprite::~BridgeMonsterSprite() {};

void BridgeMonsterSprite::customUpdate( LevelState& level_state )
{
    if ( jump_lock_ )
    {
        if ( jump_timer_.update() )
        {
            jump_lock_ = false;
            jump_start_speed_ = mezun::randInt( 2000, 500 );
            jump_top_speed_ = mezun::randInt( 8000, 5000 );
        }
    }
    else
    {
        jump();
        if ( jump_end_ )
        {
            jump_lock_ = true;
            const double dx = ( double )( Unit::BlocksToSubPixels( -2 ) );
            const double dy = ( double )( Unit::BlocksToSubPixels( 1 ) );
            level_state.sprites().spawn( std::make_unique<BridgeMonsterProjectileSprite> ( centerXPixels(), centerYPixels(), dy, dx ) );
        }       
    }
    if ( walk_timer_.update() )
    {
        direction_x_ = Direction::switchHorizontal( direction_x_ );
    }
    moveInDirectionX();
};

void BridgeMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
