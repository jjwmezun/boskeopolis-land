#include "player_plane_sprite.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

#define MAX_SPEED 3000.0
#define MIN_SPEED -2000.0
#define ACCELERATION 100.0
#define TURN_SPEED 2.0

PlayerPlaneSprite::PlayerPlaneSprite( int x, int y, std::string tileset )
:
	Sprite( std::make_unique<SpriteGraphics> ( std::move( tileset ), 0, 0, false, false, 0.0, -4, -4, 8, 8 ), x, y, 8, 8, { SpriteType::HERO }, ACCELERATION, MAX_SPEED, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, 10.0 ),
	angle_ ( 180.0 ),
	speed_ ( 0.0 )
{};

PlayerPlaneSprite::~PlayerPlaneSprite() {};

void PlayerPlaneSprite::customUpdate( LevelState& level_state )
{
	if ( Input::held( Input::Action::MOVE_RIGHT ) || Input::held( Input::Action::RUN ) || Input::held( Input::Action::MOVE_DOWN ) )
	{
		angle_ -= TURN_SPEED;
	}
	else if ( Input::held( Input::Action::MOVE_LEFT ) || Input::held( Input::Action::JUMP ) || Input::held( Input::Action::MOVE_UP ) )
	{
		angle_ += TURN_SPEED;
	}
	angle_ = ( double )( ( int )( angle_ ) % 360 );
	if ( angle_ < 0 ) { angle_ = 360 + angle_; }

	graphics_->rotation_ = angle_ + 90.0;
	speed_ += ACCELERATION;
	if ( speed_ > MAX_SPEED ) { speed_ = MAX_SPEED; }
	else if ( speed_ < -MAX_SPEED ) { speed_ = -MAX_SPEED; }
	const double radians = ( M_PI / 180.0 ) * ( angle_ - 90.0 );
	if ( ( angle_ > 0 && angle_ < 180 && vx_ >= 0 ) || ( angle_ > 180 && vx_ <= 0 ) )
	{
		vx_ = ( int )( speed_ * std::cos( radians ) );
	}
	if ( ( angle_ > 90 && angle_ < 270 && vy_ >= 0 ) || ( ( angle_ < 90 || angle_ > 270 ) && vy_ <= 0 ) )
	{
		vy_ = ( int )( speed_ * std::sin( radians ) );
	}
	Map& lvmap = level_state.currentMap();
	Camera& camera = level_state.camera();
	boundaries( camera, lvmap );
	camera.adjustCart( *this, lvmap );
	invincibilityFlicker( level_state.health() );
};

void PlayerPlaneSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::ENEMY ) && my_collision.collideAny() )
	{
		level_state.health().hurt();
	}
};
