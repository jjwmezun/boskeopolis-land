#include "lava_platform_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

LavaPlatformSprite::LavaPlatformSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/lava-platform.png" ), x, y, 64, 16, {}, 100, 6000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	activated_ ( false )
{};

LavaPlatformSprite::~LavaPlatformSprite() {};

void LavaPlatformSprite::customUpdate( LevelState& level_state )
{
	if ( activated_ )
	{
		moveInDirectionX();
		turnOnCollide();
	}
};

void LavaPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
			activated_ = true;
			them.hit_box_.x += vx_;
		}
		else
		{
			them.hit_box_.x += ( vx_ * 0.9 );
		}
	}
};
