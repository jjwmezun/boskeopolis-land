#include "collision.hpp"
#include "icecube_sprite.hpp"
#include "sprite_graphics.hpp"

IceCubeSprite::IceCubeSprite( int x, int y, Direction::Horizontal direction, bool start_moving )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/icecube.png" ), x, y, 32, 32, {}, 400, 1200, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, true, true, true ),
	start_moving_ ( start_moving ),
	start_moving_orig_ ( start_moving ),
	changed_ ( false )
{};

IceCubeSprite::~IceCubeSprite() {};

void IceCubeSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( in_water_ )
	{
		changeMovement( SpriteMovement::Type::FLOATING );

		acceleration_y_ = 0;
		vy_ = 0;

		if ( start_moving_ )
		{
			if ( direction_x_ == Direction::Horizontal::LEFT )
			{
				moveLeft();
			}
			else if ( direction_x_ == Direction::Horizontal::RIGHT )
			{
				moveRight();
			}
		}

		if ( collide_right_ || collide_left_ )
		{
			acceleration_y_ = gravity_start_speed_;
		}

	}
};

void IceCubeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
		}

		if ( their_collision.collideBottom() )
		{
			start_moving_ = true;
		}
	}
	else
	{
		if ( their_collision.collideAny() )
		{
			if ( !changed_ && start_moving_ )
			{
				direction_x_ = Direction::switchHorizontal( direction_x_ );
				changed_ = true;
			}
		}
	}
};

void IceCubeSprite::reset()
{
	resetPosition();
	changeMovement( SpriteMovement::Type::GROUNDED );
	changed_ = false;
	start_moving_ = start_moving_orig_;
	fullStopX();
	fullStopY();
	direction_x_ = direction_x_orig_;
	in_water_ = false;
};