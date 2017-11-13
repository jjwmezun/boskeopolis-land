#include "camera.hpp"
#include "collision.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "moon_sprite.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"

static constexpr int TIME_LIMIT = 45;

MoonSprite::MoonSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moon.png" ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	small_pebble_src_ ( 0, 0, 3, 3 ),
	big_pebble_src_ ( 3, 0, 5, 4 ),
	large_pebble_src_ ( 0, 4, 8, 7 ),
	moon_state_ ( MoonState::UNUSED ),
	timer_ ( 0 )
{};

MoonSprite::~MoonSprite() {};

void MoonSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( moon_state_ )
	{
		case ( MoonState::STARTING ):
			camera.startShaking();
			events.switch_ = true;
			Inventory::clock().reset( Direction::Vertical::DOWN, TIME_LIMIT );
			events.special_ = EventSystem::EType::TIMER_START;
			moon_state_ = MoonState::RUNNING;
		break;

		case ( MoonState::RUNNING ):
			for ( int i = 0; i < MAX_PEBBLES; ++i )
			{
				pebbles_[ i ].update();
			}
			if ( Inventory::clock().countdownHit0() )
			{
				events.fail();
			}
		break;
	}
	
	/*
	++timer_;
	if ( timer_ >= 8 )
	{
		timer_ = 0;
		graphics_->current_frame_x_ += 16;
		if ( graphics_->current_frame_x_ >= 12 * 16 )
		{
			graphics_->current_frame_x_ = 0;
		}
	}*/
};

void MoonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( moon_state_ == MoonState::UNUSED && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		moon_state_ = MoonState::STARTING;
		hit_box_.y = them.hit_box_.y - 8000;

		switch ( them.direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				hit_box_.x = them.hit_box_.x - 12000;
			break;
			case ( Direction::Horizontal::RIGHT ):
				hit_box_.x = them.hit_box_.right() - 4000;
			break;
		}
	}
};

void MoonSprite::render( Camera& camera, bool priority )
{
	switch ( moon_state_ )
	{
		case ( MoonState::UNUSED ):
		case ( MoonState::STARTING ):
			graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
		break;

		case ( MoonState::RUNNING ):
			for ( int i = 0; i < MAX_PEBBLES; ++i )
			{
				pebbles_[ i ].render( *this );
			}
		break;
	}
};

void MoonSprite::Pebble::update()
{
	if ( type == Type::__NULL || dest.y > Unit::WINDOW_HEIGHT_PIXELS )
	{
		regenerate();
	}
	else
	{
		dest.y += speed;
		rotation += speed * 4;
	}
};

void MoonSprite::Pebble::regenerate()
{
	const int probability = mezun::randInt( 12, 0 );

	if ( probability < 2 )
	{
		type = Type::LARGE;
		dest.w = 8;
		dest.h = 7;
	}
	else if ( probability < 6 )
	{
		type = Type::BIG;
		dest.w = 5;
		dest.h = 4;
	}
	else
	{
		type = Type::SMALL;
		dest.w = 3;
		dest.h = 3;
	}

	rotation = ( double )( mezun::randInt( 359, 0 ) );
	speed = mezun::randInt( 5, 1 );
	dest.x = mezun::randInt( Unit::WINDOW_WIDTH_PIXELS - dest.w, 0 );
	dest.y = mezun::randInt( -dest.h, -150 );
};

void MoonSprite::Pebble::render( const MoonSprite& moon ) const
{
	switch ( type )
	{
		case ( Type::SMALL ):
			Render::renderObject( "sprites/falling_pebbles.png", moon.small_pebble_src_, dest, SDL_FLIP_NONE, rotation );
		break;
		case ( Type::BIG ):
			Render::renderObject( "sprites/falling_pebbles.png", moon.big_pebble_src_, dest, SDL_FLIP_NONE, rotation );
		break;
		case ( Type::LARGE ):
			Render::renderObject( "sprites/falling_pebbles.png", moon.large_pebble_src_, dest, SDL_FLIP_NONE, rotation );
		break;
	}
};