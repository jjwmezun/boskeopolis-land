#include "weight_platform_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include "main.hpp"

#include <iostream>

static constexpr int WEIGHT_WIDTH = 48;
static constexpr int WEIGHT_HEIGHT = 8;
static constexpr int WEIGHT_SPACE_APART = 48;
static constexpr int LEFT_BAR_X = 22;
static constexpr int RIGHT_BAR_X = LEFT_BAR_X + WEIGHT_WIDTH + WEIGHT_SPACE_APART;

WeightPlatformSprite::WeightPlatformSprite( int x, int y )
:
	// Master sprite's hit box is big enough to include the full width & height o' the platforms, from the highest to lowest point.
	// This is 'cause sprite interaction & update works when the hit box is in-camera,
	// & we need to make sure the hit box is always in camera when any part o' the platforms are in camera
	// so you don't fall through any platform.
	// This is mo' efficient than having this sprite code run for every instance whether or not it's in-camera
	// & makes other calculations, like drawing the bars, simpler as well.
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/weight-platform.png" ), x, y - Unit::BlocksToPixels( 4 ), WEIGHT_WIDTH * 2 + WEIGHT_SPACE_APART, Unit::BlocksToPixels( 8 ), {}, 25, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	broken_higher_bar_ ( nullptr ),
	break_timer_ ( 0 ),
	wheel_rotation_ ( 0.0 ),
	left_ ( Unit::PixelsToSubPixels( { x, y - Unit::BlocksToPixels( 2 ), WEIGHT_WIDTH, WEIGHT_HEIGHT } ) ),
	right_ ( Unit::PixelsToSubPixels( { x + WEIGHT_WIDTH + WEIGHT_SPACE_APART, y + Unit::BlocksToPixels( 2 ), WEIGHT_WIDTH, WEIGHT_HEIGHT } ) )
{};

WeightPlatformSprite::~WeightPlatformSprite() {};

void WeightPlatformSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( isBroken() )
	{
		// Both platforms fall @ faster speed.
		top_speed_ = 3000;
		left_.y  += top_speed_;
		right_.y += top_speed_;
	}
	else
	{
		left_.y  += vy_;
		right_.y -= vy_;

		// We need to save the higher platform and its y position
		// right when the platforms break, & not any time after
		// ( as the y position will be changing after they break ),
		// so we can't just check that the platforms are broken.
		if ( break_timer_ == 15 )
		{
			broken_higher_bar_ = ( left_.y < right_.y ) ? &left_ : &right_;
		}
	}

	// We need to keep the master sprite's y the same
	// so its interactivity & update radius stay the same.
	// ( otherwise you might fall through the higher platform
	// if the camera is high 'nough ).
	// Note that master sprite's y position is changing
	// 'cause the general sprite class's update code automatically does
	// to setting acceleration_y_, & this is simpler than recreating
	// y acceleration & vy code for just the platform rects.
	hit_box_.y = original_hit_box_.y;
};

void WeightPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		bool neither_is_pressed_on = true;
		Collision left_collision = movement_->testCollision( them, left_ );
		Collision right_collision = movement_->testCollision( them, right_ );

		if ( left_collision.collideAny() )
		{
			them.collideStopAny( left_collision );
			if ( !isBroken() && left_collision.collideBottom() )
			{
				moveDown(); // Move left downward & right upward.
				neither_is_pressed_on = false;
				wheel_rotation_ -= std::abs( vy_ ) / 100 ; // Rotate left.
			}
		}
		else if ( right_collision.collideAny() )
		{
			them.collideStopAny( right_collision );
			if ( !isBroken() && right_collision.collideBottom() )
			{
				moveUp(); // Move right downward & left upward.
				neither_is_pressed_on = false;
				wheel_rotation_ += std::abs( vy_ ) / 100; // Rotate right.
			}
		}

		if ( !isBroken() )
		{
			// If left platform is at bottom limit.
			if ( left_.y + std::min( top_speed_downward_, vy_ + acceleration_y_ ) > bottomSubPixels() )
			{
				fullStopY();
				left_.y = bottomSubPixels();
				++break_timer_;
			}
			// If right platform is at bottom limit ( which is same as left platform being at top limit ).
			else if ( left_.y + std::max( -top_speed_downward_, vy_ + acceleration_y_ ) < original_hit_box_.y )
			{
				fullStopY();
				left_.y = original_hit_box_.y;
				++break_timer_;
			}
			else
			{
				break_timer_ = 0; // If we're no longer pressing down on the bottommost platform, reset break timer.
				if ( neither_is_pressed_on )
				{
					stopY();
				}
			}
		}
	}
};

void WeightPlatformSprite::render( Camera& camera, bool priority )
{
	if ( graphics_->priority_ == priority )
	{
		renderBars( camera );
		renderPlatforms( camera );
		renderWheels( camera );
	}
};

bool WeightPlatformSprite::isBroken() const
{
	return break_timer_ > 15;
};

void WeightPlatformSprite::renderPlatforms( const Camera& camera ) const
{
	graphics_->rotation_ = 0.0; // ¡Make sure we don't rotate platform graphics!
	graphics_->current_frame_x_ = 0;
	renderLeftPlatform( camera );
	renderRightPlatform( camera );
};

void WeightPlatformSprite::renderBars( const Camera& camera ) const
{
	graphics_->current_frame_x_ = 0;
	graphics_->current_frame_y_ = 8;
	graphics_->rotation_ = 0.0;
	renderTopBar( camera );
	graphics_->current_frame_x_ = 96*3;
	renderLeftBar( camera );
	renderRightBar( camera );
	graphics_->current_frame_y_ = 0;
};

void WeightPlatformSprite::renderWheels( const Camera& camera ) const
{
	graphics_->current_frame_x_ = 48; // Wheel graphic position.
	graphics_->rotation_ = wheel_rotation_;
	renderLeftWheel( camera );
	renderRightWheel( camera );
};

void WeightPlatformSprite::renderLeftPlatform( const Camera& camera ) const
{
	renderPlatform( camera, left_ );
};

void WeightPlatformSprite::renderRightPlatform( const Camera& camera ) const
{
	renderPlatform( camera, right_ );
};

void WeightPlatformSprite::renderPlatform( const Camera& camera, const sdl2::SDLRect& side ) const
{
	graphics_->render( Unit::SubPixelsToPixels( side ), &camera, graphics_->priority_ );
};

void WeightPlatformSprite::renderTopBar( const Camera& camera ) const
{
	//Render::renderRect( camera.relativeRect( { xPixels() + LEFT_BAR_X + 1, wheelMachineY(), 96, 2 } ), 2 );
	const int height = ( isBroken() && broken_higher_bar_ == &left_ ) ? 6 : Unit::SubPixelsToPixels( std::min( original_hit_box_.h, ( left_.y - original_hit_box_.y ) ) ) + 9;
	graphics_->current_frame_x_ = height;
	graphics_->render( { xPixels() + LEFT_BAR_X + 1, wheelMachineY() - 1, 96, 4 }, &camera, graphics_->priority_ );
};

void WeightPlatformSprite::renderLeftBar( const Camera& camera ) const
{
	renderSideBar( camera, LEFT_BAR_X, left_ );
};

void WeightPlatformSprite::renderRightBar( const Camera& camera ) const
{
	renderSideBar( camera, RIGHT_BAR_X, right_ );
};

void WeightPlatformSprite::renderSideBar( const Camera& camera, int x_offset, const sdl2::SDLRect& side ) const
{
	// If broken and is the platform that was higher-up when broken, make bar as high as where higher platform
	// was when the thing broke. Otherwise, make as high as top of bar thing to platform or bottom limit,
	// whichever is higher ( smaller ).
	const int height = ( isBroken() && broken_higher_bar_ == &side ) ? 6 : Unit::SubPixelsToPixels( std::min( original_hit_box_.h, ( side.y - original_hit_box_.y ) ) ) + 9;
	//Render::renderRect( camera.relativeRect( { xPixels() + x_offset, wheelMachineY() + 1, 2, height } ), 2 );
	graphics_->current_frame_y_ = (96*2) - height;
	graphics_->render( { xPixels() + x_offset, wheelMachineY() + 1, 4, height }, &camera, graphics_->priority_ );
};

void WeightPlatformSprite::renderLeftWheel( const Camera& camera ) const
{
	renderWheel( camera, LEFT_BAR_X );
};

void WeightPlatformSprite::renderRightWheel( const Camera& camera ) const
{
	renderWheel( camera, RIGHT_BAR_X - 6 );
};

void WeightPlatformSprite::renderWheel( const Camera& camera, int x_offset ) const
{
	graphics_->render( { xPixels() + x_offset, wheelMachineY(), 8, 8 }, &camera, graphics_->priority_ );
};