#include "input.hpp"
#include "shooter_player_sprite.hpp"
#include "sprite_system.hpp"

ShooterPlayerSprite::ShooterPlayerSprite( int x, int y )
:
	PlayerSprite
	(
		x,
		y,
		std::unique_ptr<InputComponent> ( new InputComponentPlayer() ),
		"sprites/autumn.png",
		SpriteType::HERO,
		160,
		2000,
		1000,
		6000,
		true
	),
	shoot_delay_count_ ( 0 )
{
	direction_x_ = Direction::Horizontal::RIGHT;
};

ShooterPlayerSprite::~ShooterPlayerSprite() {};

void ShooterPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	heroActions( camera, lvmap, events, sprites, blocks, health );

	if ( shoot_delay_count_ > 0 )
	{
		--shoot_delay_count_;
	}
	
	if ( shoot_delay_count_ == 0 && Input::pressed( Input::Action::RUN ) )
	{
		if ( lookingUp() )
		{
			sprites.spawnHeroBullet( centerXPixels(), yPixels() + 2, Direction::Simple::UP );	
		}
		else
		{
			sprites.spawnHeroBullet( centerXPixels(), yPixels() + 2, Direction::horizontalToSimple( direction_x_ ) );
		}
		shoot_delay_count_ = 16;
	}
};