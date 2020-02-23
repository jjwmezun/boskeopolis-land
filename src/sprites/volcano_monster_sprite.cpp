#include "arcing_fireball_sprite.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "volcano_monster_sprite.hpp"

static int randFireball()
{
	return mezun::randInt( 48, 16 );
};

VolcanoMonsterSprite::VolcanoMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/volcano-monster.png", 0, 0, false, false, 0, true ), x, y, 16, 17, { SpriteType::ENEMY }, 400, 800, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED ),
	timer_ ( 0 ),
	next_spit_ ( randFireball() ),
	animation_timer_ ( 0 )
{};

VolcanoMonsterSprite::~VolcanoMonsterSprite() {};

void VolcanoMonsterSprite::customUpdate( LevelState& level_state )
{
	if ( timer_ >= next_spit_ )
	{
		timer_ = 0;
		next_spit_ = randFireball();
		spawn( level_state.sprites() );
	}
	else
	{
		++timer_;
	}

	if ( animation_timer_ == 8 )
	{
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 16 : 0;
		animation_timer_ = 0;
	}
	else
	{
		++animation_timer_;
	}
};

void VolcanoMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			moveLeft();
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			moveRight();
		}
	}
};

static int randStartSpeed()
{
	return mezun::randInt( 1200, 400 );
};

static int randTopSpeed( int start_speed )
{
	return mezun::randInt( start_speed + 2400, start_speed + 400 );
};

static int randJumpStartSpeed()
{
	return mezun::randInt( 1600, 800 );
};

static int randJumpTopSpeed( int jump_start_speed )
{
	return mezun::randInt( jump_start_speed + 6000, jump_start_speed + 2000 );
};

void VolcanoMonsterSprite::spawn( SpriteSystem& sprites )
{
	const int start_speed = randStartSpeed();
	const int top_speed = randTopSpeed( start_speed );
	const int jump_start_speed = randJumpStartSpeed();
	const int jump_top_speed = randJumpTopSpeed( jump_start_speed );
	sprites.spawn( std::make_unique<ArcingFireballSprite> ( xPixels(), yPixels(), start_speed, top_speed, jump_start_speed, jump_top_speed, Direction::randomHorizontal( false ) ) );
};
