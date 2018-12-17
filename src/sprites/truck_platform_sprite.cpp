#include "truck_platform_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TruckPlatformSprite::TruckPlatformSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/truck-platform.png" ), x, y, 64, 16, { SpriteType::TRUCK_PLATFORM }, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TruckPlatformSprite::~TruckPlatformSprite() {};

void TruckPlatformSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( !jump_lock_ )
	{
		moveUp();
	}
};

void TruckPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
