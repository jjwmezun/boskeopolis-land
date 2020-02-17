#include "event_system.hpp"
#include "collision.hpp"
#include "mezun_math.hpp"
#include "move_water_sprite.hpp"
#include "sprite_graphics.hpp"

MoveWaterSprite::MoveWaterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/urban.png", 64, 48 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	spin_direction_ ( Direction::Clockwise::__NULL )
{};

MoveWaterSprite::~MoveWaterSprite() {};

void MoveWaterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( events.waterShouldMove() )
	{
		if ( events.isWaterMovingUpward() )
		{
			spin_direction_ = Direction::Clockwise::CLOCKWISE;
		}
		else if ( events.isWaterMovingDownward() )
		{
			spin_direction_ = Direction::Clockwise::COUNTERCLOCKWISE;
		}
		graphics_->rotate( spin_direction_ );
	}
	else
	{
		readjustGraphics();
	}
};

void MoveWaterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		events.setWaterMovement( bottomSubPixels() );
	}
};

void MoveWaterSprite::readjustGraphics()
{
	if ( !mezun::isAtRightAngle( ( int )( graphics_->rotation_ ) ) )
	{
		graphics_->rotate( spin_direction_ );
	}
};
