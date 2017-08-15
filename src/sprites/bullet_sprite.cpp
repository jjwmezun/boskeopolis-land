#include "collision.hpp"
#include "health.hpp"
#include "mezun_math.hpp"
#include "bullet_sprite.hpp"
#include "sprite_graphics.hpp"

BulletSprite::BulletSprite( int x, int y, Direction::Simple dir, bool heros )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", ( ( heros ) ? 5 : 0 ), 117, false, false, 0, false, 0, 0, 1, 1 ), x, y, 4, 4, bulletType( heros ), 1600, 3000, 0, 0, Direction::simpleToHorizontal( dir ), Direction::simpleToVertical( dir ), nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, true ),
	heros_ ( heros )
{};

BulletSprite::~BulletSprite() {};

void BulletSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::UP ):
			moveUp();
			graphics_->rotation_ = 90.0;
		break;

		case ( Direction::Vertical::DOWN ):
			moveDown();
			graphics_->rotation_ = 270.0;
		break;

		default:
			switch ( direction_x_ )
			{
				case ( Direction::Horizontal::RIGHT ):
					moveRight();
					graphics_->flip_x_ = true;
				break;

				case ( Direction::Horizontal::LEFT ):
					moveLeft();
					graphics_->flip_x_ = false;
				break;
			}
		break;
	}
	
	if ( collidedAny() )
	{
		kill();
	}
};

void BulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( their_collision.collideAny() )
	{
		switch ( heros_ )
		{
			case ( true ):
				if ( them.hasType( SpriteType::ENEMY ) )
				{
					them.kill();
					kill();
				}
			break;

			default:
				if ( them.hasType( SpriteType::HERO ) )
				{
					health.hurt();
					kill();
				}
			break;
		}
	}
};

std::vector<Sprite::SpriteType> BulletSprite::bulletType( bool heros )
{
	return ( heros ) ? std::vector<Sprite::SpriteType> ( { SpriteType::HEROS_BULLET } ) : std::vector<Sprite::SpriteType> ( {} );
};