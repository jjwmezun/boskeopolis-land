#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "player_spaceship_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

PlayerSpaceshipSprite::PlayerSpaceshipSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ), x, y, 14, 14, { SpriteType::HERO }, 500, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{};

PlayerSpaceshipSprite::~PlayerSpaceshipSprite() {};

void PlayerSpaceshipSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	inputMoveAllDirections();
	camera.moveRight( 1 );
	if ( !blocksJustRight( blocks ) )
	{
		hit_box_.x += 1000;
	}
	//camera.adjust( *this, lvmap );

	if ( hit_box_.x < Unit::PixelsToSubPixels( camera.x() ) )
	{
		hit_box_.x = Unit::PixelsToSubPixels( camera.x() );
	}
	else if ( hit_box_.x + hit_box_.w > Unit::PixelsToSubPixels( camera.right() ) )
	{
		hit_box_.x = Unit::PixelsToSubPixels( camera.right() ) - hit_box_.w;
	}

	containCameraY( camera );

	if ( Input::pressed( Input::Action::RUN ) && Inventory::funds() >= 100 )
	{
		sprites.spawnHeroShmupBullet( rightPixels(), centerYPixels() );
		Inventory::loseFunds( 100 );
	}
	invincibilityFlicker( health );
};

void PlayerSpaceshipSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::ENEMY ) && my_collision.collideAny() )
	{
		health.hurt();
	}
};