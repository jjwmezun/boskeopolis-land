#include "dungeon_enemy_spreader_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"

#include "input.hpp"
#include <iostream>

static constexpr double BASE_RADIANS = 0.7853982;
static constexpr double SPIKE_SPREAD_ACCELERATION = 125.0;
static constexpr double SPIKE_SPREAD_MAX_SPEED = 2000.0;
static constexpr double SPIKE_SPREAD_MAX = 150000.0;
static constexpr double SPIKE_SPREAD_MIN = 25000.0;

DungeonEnemySpreaderSprite::DungeonEnemySpreaderSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/nut-monk.png", 0, 0, false, false, 0.0, true ), x, y, 16, 16, { SpriteType::ENEMY }, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	spikes_
	({
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x - 16, y - 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x, y - 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 16, y - 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 16, y, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 16, y + 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x, y + 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x - 16, y + 16, 16, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect{ x - 16, y, 16, 16 } )
	}),
	spike_angles_
	({
		0.0,
		BASE_RADIANS * 1.0,
		BASE_RADIANS * 2.0,
		BASE_RADIANS * 3.0,
		BASE_RADIANS * 4.0,
		BASE_RADIANS * 5.0,
		BASE_RADIANS * 6.0,
		BASE_RADIANS * 7.0
	}),
	spike_radius_ ( 25000.0 ),
	spread_speed_ ( 500.0 ),
	spike_rotation_ ( 0.0 ),
	invincibility_timer_ (),
	hp_ ( 3 ),
	spikes_going_out_ ( true )
{};

DungeonEnemySpreaderSprite::~DungeonEnemySpreaderSprite() {};

void DungeonEnemySpreaderSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( hp_ <= 0 )
	{
		if ( spikes_[ 0 ].w <= 0 )
		{
			kill();
		}
		else
		{
			for ( int i = 0; i < NUMBER_OF_SPIKES; ++i )
			{
				auto& spike = spikes_[ i ];
				spikes_[ i ].w -= 1000;
				spikes_[ i ].h -= 1000;

			}
		}
	}
	else
	{
		spike_rotation_ += 5.0;
	}

	{
		for ( int i = 0; i < NUMBER_OF_SPIKES; ++i )
		{
			auto& spike = spikes_[ i ];
			const double angle = spike_angles_[ i ];
			spike.x = ( int )( ( double )( centerXSubPixels() ) - ( ( double )( spikes_[ i ].w ) / 2 ) + ( std::cos( angle ) * spike_radius_ ) );
			spike.y = ( int )( ( double )( centerYSubPixels() ) - ( ( double )( spikes_[ i ].h ) / 2 ) + ( std::sin( angle ) * spike_radius_ ) );
			spike_angles_[ i ] += 0.005;
		}

		if ( spikes_going_out_ )
		{
			spike_radius_ += spread_speed_;
			spread_speed_ += SPIKE_SPREAD_ACCELERATION;
			if ( spread_speed_ > SPIKE_SPREAD_MAX_SPEED )
			{
				spread_speed_ = SPIKE_SPREAD_MAX_SPEED;
			}
			if ( spike_radius_ >= SPIKE_SPREAD_MAX )
			{
				spikes_going_out_ = false;
				spike_radius_ = SPIKE_SPREAD_MAX;
				spread_speed_ = SPIKE_SPREAD_ACCELERATION;
			}
		}
		else
		{
			spike_radius_ -= spread_speed_;
			spread_speed_ += SPIKE_SPREAD_ACCELERATION;
			if ( spread_speed_ > SPIKE_SPREAD_MAX_SPEED )
			{
				spread_speed_ = SPIKE_SPREAD_MAX_SPEED;
			}
			if ( spike_radius_ <= SPIKE_SPREAD_MIN )
			{
				spikes_going_out_ = true;
				spike_radius_ = SPIKE_SPREAD_MIN;
				spread_speed_ = SPIKE_SPREAD_ACCELERATION;
			}
		}
		updateInvincibility();
	}
};

void DungeonEnemySpreaderSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.hit_box_.x > rightSubPixels() )
		{
			moveRight();
		}
		else if ( them.rightSubPixels() < hit_box_.x )
		{
			moveLeft();
		}

		if ( them.hit_box_.y > bottomSubPixels() )
		{
			moveDown();
		}
		else if ( them.bottomSubPixels() < hit_box_.y )
		{
			moveUp();
		}

		for ( int i = 0; i < NUMBER_OF_SPIKES; ++i )
		{
			const auto collision = them.testCollision( spikes_[ i ] );
			if ( collision.collideAny() )
			{
				health.hurt();
				return;
			}
		}
	}
	else if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
	{
		hurt();
	}
};

void DungeonEnemySpreaderSprite::render( Camera& camera, bool priority )
{
	if ( !invincibilityFlickerOff() )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
	for ( int i = 0; i < NUMBER_OF_SPIKES; ++i )
	{
		Render::renderObject( "sprites/nut-monk.png", { 0, 16, 16, 16 }, Unit::SubPixelsToPixels( spikes_[ i ] ), SDL_FLIP_NONE, spike_rotation_, 255, &camera );
	}
};

void DungeonEnemySpreaderSprite::updateInvincibility()
{
	if ( invincibility_timer_.on() )
	{
		invincibility_timer_.update();
	}
}

void DungeonEnemySpreaderSprite::hurt()
{
	if ( hp_ > 0 && !invincibility_timer_.on() )
	{
		--hp_;
		invincibility_timer_.start();
	}
}

bool DungeonEnemySpreaderSprite::invincibilityFlickerOff() const
{
	return invincibility_timer_.counter() % 4 == 1;
}
