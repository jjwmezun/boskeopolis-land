#include "mezun_math.hpp"
#include "snowman_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

SnowmanSprite::SnowmanSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowman.png", 8, 0, false, false, 0, false, -1, -2, 2, 4 ), x, y, 15, 22, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 1000, jumpHeight(), jumpHeight(), Direction::Horizontal::LEFT ),
	move_type_ ( moveType() ),
	move_time_ ( moveTime() ),
	move_counter_ ( 0 ),
	jump_time_ ( jumpTime() ),
	jump_counter_ ( 0 ),
	jump_ ( false ),
	throw_time_ ( 0 ),
	throw_counter_ ( 0 )
{
	jump_lock_ = false;
};

SnowmanSprite::~SnowmanSprite() {};

void SnowmanSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
		break;
			
		default:
			graphics_->flip_x_ = false;
		break;
	}
	
	handleMovement();
	handleJumping();
	handleThrowing( sprites );
};

void SnowmanSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.xSubPixels() > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}
		else
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
	}
};

int SnowmanSprite::jumpHeight() const
{
	return mezun::randInt( 8000, 3000 );
};

int SnowmanSprite::jumpTime() const
{
	return mezun::randInt( 180, 30 );
};

void SnowmanSprite::handleJumping()
{
	if ( jump_counter_ >= jump_time_ && onGround() )
	{
		jump();
		jump_counter_ = 0;
		jump_time_ = jumpTime();
		jump_start_speed_ = jumpHeight();
		jump_top_speed_ = jumpHeight();
	}
	++jump_counter_;
};

int SnowmanSprite::moveTime() const
{
	return mezun::randInt( 32, 8 );
};

int SnowmanSprite::moveType() const
{
	return mezun::randInt( 3, 0 );
}

void SnowmanSprite::handleMovement()
{
	if ( move_counter_ >= move_time_ )
	{
		move_type_ = moveType();
		move_time_ = moveTime();
		move_counter_ = 0;
	}

	switch ( move_type_ )
	{
		case ( 3 ):
			hit_box_.x -= 1000;
		break;	

		case ( 2 ):
			hit_box_.x += 1000;
		break;
			
		default:
			fullStopX();
		break;
	}
	
	++move_counter_;
};

int SnowmanSprite::throwTime() const
{
	return mezun::randInt( 80, 10 );
};

void SnowmanSprite::handleThrowing( SpriteSystem& sprites )
{
	if ( throw_counter_ >= throw_time_ )
	{
		sprites.spawnSnowball( centerXPixels(), yPixels() + 8, direction_x_ );
		throw_counter_ = 0;
		throw_time_ = throwTime();
	}
	++throw_counter_;
};