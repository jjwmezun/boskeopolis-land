#include "block_system.hpp"
#include "camera.hpp"
#include <cmath>
#include "input.hpp"
#include "player_car_sprite.hpp"
#include "sprite_graphics.hpp"
#include "test_line_and_box_collision.hpp"

#include <iostream>

#define MAX_SPEED 8000.0
#define MIN_SPEED -2000.0
#define START_SPEED 100.0
#define BACK_SPEED -50.0
#define TURN_SPEED 2.0

PlayerCarSprite::PlayerCarSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn_car.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 18, 28, { SpriteType::HERO }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::ANGLED, CameraMovement::PERMANENT ),
	angle_ ( 0.0 ),
	speed_ ( 0.0 ),
	acceleration_ ( 0.0 ),
	block_list_ ( {} )
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

	double speed = speed_ + acceleration_;
	if ( Input::held( Input::Action::CANCEL ) )
	{
		speed /= 1.15;
	}

	double angle = angle_;
	if ( speed >= START_SPEED || speed <= BACK_SPEED )
	{
		if ( Input::held( Input::Action::MOVE_LEFT ) )
		{
			angle -= TURN_SPEED;
		}
		else if ( Input::held( Input::Action::MOVE_RIGHT ) )
		{
			angle += TURN_SPEED;
		}
	}

	if ( speed > MAX_SPEED ) speed = MAX_SPEED;
	if ( speed < MIN_SPEED ) speed = MIN_SPEED;

	const double angle_radians = mezun::convertDegreesToRadians( angle - 90.0 );
	int x = hit_box_.x + ( int )( speed * std::cos( angle_radians ) );
	int y = hit_box_.y + ( int )( speed * std::sin( angle_radians ) );

	bool collisions = testForCollisions( x, y, camera, blocks, events, health );

	if ( collisions )
	{
		std::cout<<"HIT"<<std::endl;
		if ( angle_ < angle )
		{
			angle_ -= ( 3 * TURN_SPEED );
		}
		else if ( angle_ > angle )
		{
			angle_ += ( 3 * TURN_SPEED );
		}
		speed = speed_ * ( -2.0 );

		if ( speed > MAX_SPEED ) speed = MAX_SPEED;
		if ( speed < MIN_SPEED ) speed = MIN_SPEED;

		x = hit_box_.x + ( int )( speed * std::cos( angle_radians ) );
		y = hit_box_.y + ( int )( speed * std::sin( angle_radians ) );
		collisions = testForCollisions( x, y, camera, blocks, events, health );

		if ( collisions )
		{
			speed_ = 0.0;
		}
		else
		{
			speed_ = speed;
		}
	}
	else
	{
		hit_box_.x = x;
		hit_box_.y = y;
		angle_ = angle;
		speed_ = speed;
	}
	speed_ /= 1.005;
	std::cout<<speed_<<std::endl;
	graphics_->rotation_ = angle_;

	boundaries( camera, lvmap );
	camera.adjust( *this, lvmap );
};

void PlayerCarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};

void PlayerCarSprite::render( Camera& camera, bool priority )
{
	for ( const Block* block : block_list_ )
	{
		const sdl2::SDLRect relative_box = camera.relativeRect( Unit::SubPixelsToPixels( block->hit_box_ ) );
		Render::renderRectDebug( relative_box, { 255, 0, 0, 255 } );
	}

	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	const auto lines = AngledSpriteMovement::getLinesFromBox( hit_box_, mezun::convertDegreesToRadians( angle_ ) );
	for ( const Line& line : lines )
	{
		const Line relative_line = camera.relativeLine( Unit::SubPixelsToPixels( line ) );
		Render::renderLineDebug( relative_line, { 0, 0, 255, 128 } );
	}
};

bool PlayerCarSprite::testForCollisions( int x, int y, const Camera& camera, const BlockSystem& blocks, const EventSystem& events, const Health& health )
{
	const sdl2::SDLRect new_hit_box = { x, y, hit_box_.w, hit_box_.h, };
	const auto block_list = blocks.getSolidBlocksInField( new_hit_box, camera, *this, events, health );
	block_list_ = block_list;
	const auto collision_lines = AngledSpriteMovement::getLinesFromBox( new_hit_box, mezun::convertDegreesToRadians( angle_ ) );
	for ( const Block* block : block_list )
	{
		for ( const Line& line : collision_lines )
		{
			if ( mezun::testLineAndBoxCollision( line, block->hit_box_ ) )
			{
				return true;
			}
		}
	}
	return false;
};