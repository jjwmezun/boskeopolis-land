#include "block_system.hpp"
#include "jump_and_shoot_sprite.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int jumpHeight();
static int jumpAcceleration();

JumpAndShootSprite::JumpAndShootSprite( int x, int y, int w, int h, std::unique_ptr<SpriteGraphics>&& gfx, void ( *throw_function )( const Sprite& me, SpriteSystem& sprites ) )
:
	Sprite( std::move( gfx ), x, y, w, h, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 1000, jumpAcceleration(), jumpHeight(), Direction::Horizontal::LEFT ),
	move_timer_ (),
	throw_timer_ (),
	jump_timer_ (),
	throw_function_ ( throw_function ),
	move_direction_ ( Direction::randomHorizontal() )
{};

JumpAndShootSprite::~JumpAndShootSprite() {};

void JumpAndShootSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	flipGraphicsOnRight();
	if ( !handleJumping() )
	{
		handleMovement( blocks );
	}
	handleThrowing( sprites );
};

void JumpAndShootSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		direction_x_ = ( them.xSubPixels() > rightSubPixels() )
			? Direction::Horizontal::RIGHT
			: Direction::Horizontal::LEFT;
	}
};

bool JumpAndShootSprite::handleJumping()
{
	if ( onGround() && jump_timer_.update() )
	{
		jump();
		jump_start_speed_ = jumpAcceleration();
		jump_top_speed_ = jumpHeight();
		return true;
	}
	else if ( !onGround() )
	{
		jump();
		return true;
	}
	return false;
};

void JumpAndShootSprite::handleMovement( const BlockSystem& blocks )
{
	if ( move_timer_.update() )
	{
		move_direction_ = Direction::randomHorizontal();
	}

	switch ( move_direction_ )
	{
		case ( Direction::Horizontal::__NULL ):
		{
			fullStopX();
		}
		break;

		case ( Direction::Horizontal::LEFT ):
		{
			if
			(
				blocks.blocksInTheWay( { hit_box_.x - 16000, bottomSubPixels(), 16000, 16000 }, BlockComponent::Type::SOLID ) &&
				!blocks.blocksInTheWay( { hit_box_.x - 1000, hit_box_.y, 1000, hit_box_.h }, BlockComponent::Type::SOLID )
			)
			{
				hit_box_.x -= 1000;
			}
		}
		break;

		case ( Direction::Horizontal::RIGHT ):
		{
			if
			(
				blocks.blocksInTheWay( { rightSubPixels(), bottomSubPixels(), 16000, 16000 }, BlockComponent::Type::SOLID ) &&
				!blocks.blocksInTheWay( { rightSubPixels(), hit_box_.y, 1000, hit_box_.h }, BlockComponent::Type::SOLID )
			)
			{
				hit_box_.x += 1000;
			}
		}
		break;
	}
};

void JumpAndShootSprite::handleThrowing( SpriteSystem& sprites )
{
	if ( throw_timer_.update() )
	{
		throw_function_( *this, sprites );
	}
};

static int jumpHeight()
{
	return mezun::randInt( 6000, 3000 );
};

static int jumpAcceleration()
{
	return mezun::randInt( 500, 1500 );
};
