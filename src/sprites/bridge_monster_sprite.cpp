#include "bridge_monster_projectile_sprite.hpp"
#include "bridge_monster_sprite.hpp"
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

void BridgeMonsterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    if ( jump_lock_ )
    {
        if ( jump_timer_.update() )
        {
            jump_lock_ = false;
            jump_start_speed_ = mezun::randInt( 2000, 500 );
            jump_top_speed_ = mezun::randInt( 8000, 3000 );
        }
    }
    else
    {
        jump();
        if ( jump_end_ )
        {
            jump_lock_ = true;
            shoot_ = true;
        }       
    }
    if ( walk_timer_.update() )
    {
        direction_x_ = Direction::switchHorizontal( direction_x_ );
    }
    moveInDirectionX();
};

void BridgeMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
    if ( shoot_ && them.hasType( SpriteType::HERO ) )
    {
        const double dx = ( double )( them.centerXSubPixels() - centerXSubPixels() );
        const double dy = ( double )( them.centerYSubPixels() - centerYSubPixels() );
        sprites.spawn( std::make_unique<BridgeMonsterProjectileSprite> ( centerXPixels(), centerYPixels(), dy, dx ) );
        shoot_ = false;
    }
};
