#include "camera.hpp"
#include <cmath>
#include "input.hpp"
#include "player_doom_sprite.hpp"

static constexpr double DEG_ROTATION_SPEED = mezun::DOUBLE_PI * 2;
static constexpr double ROTATION_SPEED = DEG_ROTATION_SPEED / 360.0; // 1° in radians.
static constexpr double N90DEGREES = mezun::DOUBLE_PI / 4; // 90°

PlayerDoomSprite::PlayerDoomSprite( int x, int y )
:
	Sprite
	(
		nullptr,
		x,
		y,
		16,
		16,
		{ SpriteType::HERO },
		200, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true, true, 0
	),
	posx_ ( 0 ),
	posy_ ( 0 ),
	ddirx_ ( -1 ),
	ddiry_ ( 0 ),
	planex_ ( 0 ),
	planey_ ( 0.66 ),
	prevposx_ ( 0 ),
	prevposy_ ( 0 ),
	prevdirx_ ( 0 ),
	prevdiry_ ( 0 ),
	angle_ ( 0 )
{
	direction_x_ = ( Direction::Horizontal )( ddirx_ * CONVERSION_PRECISION );
	direction_y_ = ( Direction::Vertical )( ddiry_ * CONVERSION_PRECISION );
	jump_top_speed_ = ( int )( planex_ * CONVERSION_PRECISION );
	jump_top_speed_normal_ = ( int )( planey_ * CONVERSION_PRECISION );
	jump_lock_ = true;
	bounce_height_ = ( int )( angle_ * CONVERSION_PRECISION );
};

PlayerDoomSprite::~PlayerDoomSprite() {};

void PlayerDoomSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		acceleration_y_ = getAccelerationAdjustedByAngle( ddiry_ );
		acceleration_x_ = getAccelerationAdjustedByAngle( ddirx_ );
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		acceleration_y_ = -getAccelerationAdjustedByAngle( ddiry_ );
		acceleration_x_ = -getAccelerationAdjustedByAngle( ddirx_ );
	}
	else
	{
		acceleration_y_ = acceleration_x_ = 0;
	}

	if ( Input::held( Input::Action::CAMERA_RIGHT ) )
	{
		moveSideways( 1.0 );
	}
	else if ( Input::held( Input::Action::CAMERA_LEFT ) )
	{
		moveSideways( -1.0 );
	}

	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		rotate( -ROTATION_SPEED );
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		rotate( ROTATION_SPEED );
	}

	jump_lock_ = ddirx_ != prevdirx_ || ddiry_ != prevdiry_ || hit_box_.x != prevposx_ || hit_box_.y != prevposy_;
	prevdirx_ = ddirx_;
	prevdiry_ = ddiry_;
	prevposx_ = hit_box_.x;
	prevposy_ = hit_box_.y;
	camera.adjustCart( *this, lvmap );
};

void PlayerDoomSprite::rotate( double rotation_speed )
{
	const double old_dirx = ddirx_;
	const double old_planex = planex_;
	ddirx_ = ddirx_ * cos( rotation_speed ) - ddiry_ * sin( rotation_speed );
	ddiry_ = old_dirx * sin( rotation_speed ) + ddiry_ * cos( rotation_speed );
	planex_ = planex_ * cos( rotation_speed ) - planey_ * sin( rotation_speed );
	planey_ = old_planex * sin( rotation_speed ) + planey_ * cos( rotation_speed );
	angle_ += rotation_speed * 60.0;

	// MapLayerDoom can only recognize this is a generic sprite, thanks to polymorphism.
	// These are generic sprite values, so it can reach these.
	direction_x_ = ( Direction::Horizontal )( ddirx_ * CONVERSION_PRECISION );
	direction_y_ = ( Direction::Vertical )( ddiry_ * CONVERSION_PRECISION );
	jump_top_speed_ = ( int )( planex_ * CONVERSION_PRECISION );
	jump_top_speed_normal_ = ( int )( planey_ * CONVERSION_PRECISION );
	bounce_height_ = ( int )( angle_ * CONVERSION_PRECISION );
}

void PlayerDoomSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};

void PlayerDoomSprite::render( Camera& camera, bool priority )
{
	//Render::renderRectDebug( Unit::SubPixelsToPixels( hit_box_ ), { 255, 0, 0, 255 } );
};

double PlayerDoomSprite::getAccelerationAdjustedByAngle( double angle )
{
	return ( int )( ( double )( start_speed_ ) * angle );
};

void PlayerDoomSprite::moveSideways( double multiplier )
{
	const double right_angle_x = ddirx_ * cos( N90DEGREES ) - ddiry_ * sin( N90DEGREES );
	const double right_angle_y = ddirx_ * sin( N90DEGREES ) + ddiry_ * cos( N90DEGREES );
	const int y_change = multiplier * getAccelerationAdjustedByAngle( right_angle_y );
	const int x_change = multiplier * getAccelerationAdjustedByAngle( right_angle_x );
	acceleration_y_ += y_change;
	acceleration_x_ += x_change;
}
