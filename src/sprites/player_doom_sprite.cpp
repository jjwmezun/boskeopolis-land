#include "audio.hpp"
#include "camera.hpp"
#include <cmath>
#include "doom_bullet_sprite.hpp"
#include "health.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "player_doom_sprite.hpp"
#include "sprite_system.hpp"

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
	screen_tint_ ( { 255, 0, 0, 0 } ),
	posx_ ( 0 ),
	posy_ ( 0 ),
	ddirx_ ( -1 ),
	ddiry_ ( 0 ),
	planex_ ( 0 ),
	planey_ ( 0.66 ),
	prevdirx_ ( 0 ),
	prevdiry_ ( 0 ),
	angle_ ( 0 ),
	prevposx_ ( 0 ),
	prevposy_ ( 0 ),
	shoot_timer_ ( 0 ),
	has_moved_ ( true )
{};

PlayerDoomSprite::~PlayerDoomSprite() {};

void PlayerDoomSprite::customUpdate( LevelState& level_state )
{
	const bool is_running = Input::held( Input::Action::RUN );
	start_speed_ = ( is_running ) ? 2000 : 1000;
	top_speed_ = ( is_running ) ? 2000 : 1000;

	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		vx_ = getAccelerationAdjustedByAngle( ddirx_ );
		vy_ = getAccelerationAdjustedByAngle( ddiry_ );
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		vx_ = -getAccelerationAdjustedByAngle( ddirx_ );
		vy_ = -getAccelerationAdjustedByAngle( ddiry_ );
	}

	if ( Input::held( Input::Action::CAMERA_RIGHT ) )
	{
		moveStraight( 1.0, N90DEGREES );
	}
	else if ( Input::held( Input::Action::CAMERA_LEFT ) )
	{
		moveStraight( -1.0, N90DEGREES );
	}

	if ( Input::held( Input::Action::CAMERA_UP ) )
	{
		moveStraight( -1.0, N90DEGREES * 2 );
	}
	else if ( Input::held( Input::Action::CAMERA_DOWN ) )
	{
		moveStraight( 1.0, N90DEGREES * 2 );
	}

	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		rotate( -ROTATION_SPEED );
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		rotate( ROTATION_SPEED );
	}

	if ( shoot_timer_ == 0 )
	{
		if ( Input::held( Input::Action::JUMP ) )
		{
			shoot_timer_ = 32;
		}
	}
	else
	{
		--shoot_timer_;
		if ( shoot_timer_ == 16 )
		{
			Audio::playSound( Audio::SoundType::JUMP );
			const int movement_x = ( int )( 32000.0 * ddirx_ );
			const int movement_y = ( int )( 32000.0 * ddiry_ );
			const int x = Unit::SubPixelsToPixels( hit_box_.x + movement_x );
			const int y = Unit::SubPixelsToPixels( hit_box_.y + movement_y );
			level_state.sprites().spawn( std::unique_ptr<Sprite> ( new DoomBulletSprite( x, y, ddirx_, ddiry_ ) ) );
		}
	}

	has_moved_ = ddirx_ != prevdirx_ || ddiry_ != prevdiry_ || hit_box_.x != prevposx_ || hit_box_.y != prevposy_;
	prevdirx_ = ddirx_;
	prevdiry_ = ddiry_;
	prevposx_ = hit_box_.x;
	prevposy_ = hit_box_.y;

	updateHurtAnimation( level_state.health() );
	level_state.camera().adjustCart( *this, level_state.currentMap() );
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
}

void PlayerDoomSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void PlayerDoomSprite::render( Camera& camera, bool priority )
{
	Render::tintScreen( screen_tint_ );
};

double PlayerDoomSprite::getAccelerationAdjustedByAngle( double angle )
{
	return ( int )( ( double )( start_speed_ ) * angle );
};

void PlayerDoomSprite::moveStraight( double multiplier, double angle )
{
	const double right_angle_x = ddirx_ * cos( angle ) - ddiry_ * sin( angle );
	const double right_angle_y = ddirx_ * sin( angle ) + ddiry_ * cos( angle );
	const int y_change = multiplier * getAccelerationAdjustedByAngle( right_angle_y );
	const int x_change = multiplier * getAccelerationAdjustedByAngle( right_angle_x );
	vx_ += x_change;
	vy_ += y_change;
}

void PlayerDoomSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	fullStopX();
	fullStopY();
	block_interact_ = false;
	sprite_interact_ = false;
	screen_tint_.a += 2;
	if ( screen_tint_.a >= 254 )
	{
		death_finished_ = true;
	}
};

void PlayerDoomSprite::updateHurtAnimation( const Health& health )
{
	if ( health.getInvincibilityCounter() > 0 )
	{
		if ( health.getInvincibilityCounter() > 24 )
		{
			// After midpoint o' timer, alpha goes back downward ( less red ) as counter rises.
			screen_tint_.a = ( Uint8 )( 255.0 - ( ( 255.0 / 24.0 ) * ( double )( health.getInvincibilityCounter() ) ) );
		}
		else
		{
			// Else, before midpoint, alpha goes up ( mo' red ) as counter rises.
			screen_tint_.a = ( Uint8 )( ( 255.0 / 24.0 ) * ( double )( health.getInvincibilityCounter() ) );
		}
	}
}
