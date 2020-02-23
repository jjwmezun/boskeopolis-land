#include "audio.hpp"
#include "spring_enemy_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int BOUNCE_STRENGTH = 16;

SpringEnemySprite::SpringEnemySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/sillyfish.png" ), x, y, 16, 32, {}, 200, 4000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

SpringEnemySprite::~SpringEnemySprite() {};

void SpringEnemySprite::customUpdate( LevelState& level_state )
{};

void SpringEnemySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			moveLeft();
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			moveRight();
		}
	}

	if ( them.collideBottomOnly( their_collision, *this ) )
	{
		them.bounce( BOUNCE_STRENGTH );
	}
	else if ( their_collision.collideLeft() )
	{
		them.bounceLeft( vx_ );
		bounceRight( them.vx_ );
	}
	else if ( their_collision.collideRight() )
	{
		them.bounceRight( vx_ );
		bounceLeft( them.vx_ );
	}
	else if ( their_collision.collideTop() )
	{
		them.bounceDownward( BOUNCE_STRENGTH );
	}

	if ( their_collision.collideAny() )
	{
		Audio::playSound( Audio::SoundType::BOUNCE );
	}
};
