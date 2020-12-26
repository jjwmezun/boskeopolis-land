#include "camera.hpp"
#include "change_camera_autoscroll_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

ChangeCameraAutoscrollSprite::ChangeCameraAutoscrollSprite( int x, int y, Direction::Simple direction )
:
	Sprite( nullptr, x, y, Unit::BlocksToPixels( 3 ), Unit::BlocksToPixels( 3 ), { SpriteType::CAMERA_MOVE }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true ),
	direction_ ( direction )
{};

ChangeCameraAutoscrollSprite::~ChangeCameraAutoscrollSprite() {};

void ChangeCameraAutoscrollSprite::customUpdate( LevelState& level_state )
{
	/*
	const Direction::Simple movement_speed = ( Direction::Simple )( top_speed_ );
	Camera& camera = level_state.camera();
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		case ( Direction::Simple::DOWN ):
		{
			const int camera_center = ( int )( floor( ( float )( camera.x() ) + ( ( float )( camera.width() ) / 2 ) ) );
			const bool condition = ( movement_speed == Direction::Simple::RIGHT ) ? centerXPixels() <= camera_center : centerXPixels() >= camera_center;
			if ( condition )
			{
				direction_y_ = Direction::simpleToVertical( direction_ );
			}
		}
		break;

		case ( Direction::Simple::RIGHT ):
		case ( Direction::Simple::LEFT ):
		{
			const int camera_center = ( int )( floor( ( float )( camera.y() ) + ( ( float )( camera.height() ) / 2 ) ) );
			const bool condition = ( movement_speed == Direction::Simple::DOWN ) ? centerYPixels() <= camera_center : centerYPixels() >= camera_center;
			if ( condition )
			{
				direction_x_ = Direction::simpleToHorizontal( direction_ );
			}
		}
		break;
	}
	*/
};

void ChangeCameraAutoscrollSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		switch ( direction_ )
		{
			case ( Direction::Simple::UP ):
			case ( Direction::Simple::DOWN ):
			{
				direction_y_ = Direction::simpleToVertical( direction_ );
			}
			break;

			case ( Direction::Simple::RIGHT ):
			case ( Direction::Simple::LEFT ):
			{
				direction_x_ = Direction::simpleToHorizontal( direction_ );
			}
			break;
		}
	}
};
