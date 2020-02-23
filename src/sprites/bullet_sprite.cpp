#include "audio.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "bullet_sprite.hpp"
#include "sprite_graphics.hpp"

static std::vector<Sprite::SpriteType> bulletType( bool heros );

BulletSprite::BulletSprite( int x, int y, Direction::Simple dir, bool heros )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", ( ( heros ) ? 5 : 0 ), 117, false, false, 0, false, 0, 0, 1, 1 ), x, y, 4, 4, bulletType( heros ), 1600, 3000, 0, 0, Direction::simpleToHorizontal( dir ), Direction::simpleToVertical( dir ), nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, true )
{};

BulletSprite::~BulletSprite() {};

void BulletSprite::customUpdate( LevelState& level_state )
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

void BulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		switch ( hasType( SpriteType::HEROS_BULLET ) )
		{
			case ( true ):
				if ( them.hasType( SpriteType::ENEMY ) )
				{
					them.kill();
					kill();
					Audio::playSound( Audio::SoundType::BOP );
				}
			break;

			default:
				if ( them.hasType( SpriteType::HERO ) )
				{
					level_state.health().hurt();
					kill();
				}
			break;
		}
	}
};

std::vector<Sprite::SpriteType> bulletType( bool heros )
{
	return ( heros ) ? std::vector<Sprite::SpriteType> ( { Sprite::SpriteType::HEROS_BULLET } ) : std::vector<Sprite::SpriteType> ( {} );
};