#include <cmath>
#include "attic_monster_projectile_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#define SPEED 2000.0

AtticMonsterProjectileSprite::AtticMonsterProjectileSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/attic-rat.png", 88, 0, false, false, 0.0, -1, -1, 2, 2 ), x, y, 4, 4, { SpriteType::ENEMY }, 3000, 3000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true, false ),
	rotation_ ( ( direction == Direction::Horizontal::LEFT ) ? -2.0 : 2.0 )
{
	layer_ = Unit::Layer::SPRITES_2;
};

AtticMonsterProjectileSprite::~AtticMonsterProjectileSprite() {};

void AtticMonsterProjectileSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();
	graphics_->rotation_ += rotation_;
};

void AtticMonsterProjectileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
