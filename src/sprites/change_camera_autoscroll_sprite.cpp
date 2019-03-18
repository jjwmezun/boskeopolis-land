#include "camera.hpp"
#include "change_camera_autoscroll_sprite.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

ChangeCameraAutoscrollSprite::ChangeCameraAutoscrollSprite( int x, int y, Direction::Simple direction )
:
	Sprite( nullptr, x, y, 48, 128, { SpriteType::CAMERA_MOVE }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true ),
	direction_ ( direction )
{};

ChangeCameraAutoscrollSprite::~ChangeCameraAutoscrollSprite() {};

void ChangeCameraAutoscrollSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	const Direction::Simple movement_speed = ( Direction::Simple )( top_speed_ );
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		case ( Direction::Simple::DOWN ):
		{
			const int camera_center = ( int )( floor( ( float )( camera.x() ) + ( ( float )( camera.widthPixels() ) / 2 ) ) );
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
			std::cout<<(int)movement_speed<<std::endl;
			const int camera_center = ( int )( floor( ( float )( camera.y() ) + ( ( float )( camera.heightPixels() ) / 2 ) ) );
			const bool condition = ( movement_speed == Direction::Simple::DOWN ) ? centerYPixels() <= camera_center : centerYPixels() >= camera_center;
			if ( condition )
			{
				direction_x_ = Direction::simpleToHorizontal( direction_ );
			}
		}
		break;
	}
};

void ChangeCameraAutoscrollSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};
