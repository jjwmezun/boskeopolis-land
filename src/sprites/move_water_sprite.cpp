#include "event_system.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "move_water_sprite.hpp"
#include "sprite_graphics.hpp"

MoveWaterSprite::MoveWaterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/urban.png", 64, 48 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	spin_direction_ ( Direction::Clockwise::__NULL )
{};

MoveWaterSprite::~MoveWaterSprite() {};

void MoveWaterSprite::customUpdate( LevelState& level_state )
{
	EventSystem& events = level_state.events();
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

void MoveWaterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		level_state.events().setWaterMovement( bottomSubPixels() );
	}
};

void MoveWaterSprite::readjustGraphics()
{
	if ( !mezun::isAtRightAngle( ( int )( graphics_->rotation_ ) ) )
	{
		graphics_->rotate( spin_direction_ );
	}
};
