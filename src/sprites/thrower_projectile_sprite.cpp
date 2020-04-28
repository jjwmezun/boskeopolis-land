#include "thrower_projectile_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

ThrowerProjectileSprite::ThrowerProjectileSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 8, 8, { SpriteType::ENEMY }, 0, 0, 2000, 6000, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, false )
{
    on_ground_ = true;
    start_speed_ = mezun::randInt( 3000, 1000 );
    if ( direction == Direction::Horizontal::RIGHT )
    {
        start_speed_ *= -1;
    }
};

ThrowerProjectileSprite::~ThrowerProjectileSprite() {};

void ThrowerProjectileSprite::customUpdate( LevelState& level_state )
{
    jump();
    hit_box_.x -= start_speed_;
    if ( fellInBottomlessPit( level_state.currentMap() ) )
    {
        kill();
    }
};

void ThrowerProjectileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
