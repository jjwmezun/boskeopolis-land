#include "block_system.hpp"
#include "camera.hpp"
#include <cmath>
#include "input.hpp"
#include "player_car_sprite.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

#define MAX_SPEED 8000.0
#define MIN_SPEED -2000.0
#define START_SPEED 100.0
#define BACK_SPEED -50.0
#define TURN_SPEED 2.0

PlayerCarSprite::PlayerCarSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn_car.png" ), x, y, 22, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::ANGLED, CameraMovement::PERMANENT ),
	angle_ ( 0.0 ),
	speed_ ( 0.0 ),
	acceleration_ ( 0.0 )
{};

PlayerCarSprite::~PlayerCarSprite() {};

void PlayerCarSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		acceleration_ = START_SPEED;
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		acceleration_ = BACK_SPEED;
	}
	else
	{
		acceleration_ = 0;
	}

	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		angle_ -= TURN_SPEED;
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		angle_ += TURN_SPEED;
	}
	graphics_->rotation_ = angle_;

	speed_ += acceleration_;

	if ( speed_ > MAX_SPEED ) speed_ = MAX_SPEED;
	if ( speed_ < MIN_SPEED ) speed_ = MIN_SPEED;

	const double radians = ( M_PI / 180.0 ) * ( angle_ - 90.0 );
	const int temp_x = hit_box_.x;
	const int temp_y = hit_box_.y;
	hit_box_.x += ( int )( speed_ * std::cos( radians ) );
	hit_box_.y += ( int )( speed_ * std::sin( radians ) );
	
	if ( blocks.blocksInTheWay( hit_box_, BlockComponent::Type::SOLID ) )
	{
		hit_box_.x = temp_x;
		hit_box_.y = temp_y;
		speed_ = -speed_;
	}
	
	speed_ /= 1.005;

	boundaries( camera, lvmap );
	camera.adjust( *this, lvmap );
};

void PlayerCarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};