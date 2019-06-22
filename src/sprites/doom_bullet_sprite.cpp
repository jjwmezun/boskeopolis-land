#include <cmath>
#include "doom_bullet_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

DoomBulletSprite::DoomBulletSprite( int x, int y, double dx, double dy )
:
	Sprite( nullptr, x, y, 8, 8, { SpriteType::HEROS_BULLET }, 500, 2500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, true ),
	dx_ ( dx ),
	dy_ ( dy )
{};

DoomBulletSprite::~DoomBulletSprite() {};

void DoomBulletSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	vx_ = 2500.0 * dx_;
	vy_ = 2500.0 * dy_;
	if ( collide_top_ || collide_bottom_ || collide_left_ || collide_right_ )
	{
		kill();
	}
};

void DoomBulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::ENEMY ) && their_collision.collideAny() )
	{
		them.kill();
		kill();
	}
};

void DoomBulletSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	death_finished_ = true;
};
