#include "audio.hpp"
#include "mezun_math.hpp"
#include "cannon_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

CannonSprite::CannonSprite( int x, int y, Direction::Vertical dir, int map_id )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", 0, typeGFX( dir ), false, false, 0, true, 0, 0, 0, 0 ), x, y, 16, 16, { SpriteType::DEATH_COUNT }, 100, 2000, 0, 0, Direction::Horizontal::__NULL, dir, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, false, 2.0, false, false, map_id ),
	deactivated_ ( false ),
	shoot_time_ ( 0 ),
	shoot_counter_ ( 0 ),
	hp_ ( 4 ),
	flash_time_ ( 0 )
{};

CannonSprite::~CannonSprite() {};

int CannonSprite::typeGFX( Direction::Vertical dir )
{
	switch ( dir )
	{
		case ( Direction::Vertical::UP ):
			return 0;
		break;

		case ( Direction::Vertical::DOWN ):
			return 16;
		break;

		case ( Direction::Vertical::__NULL ):
			return 32;
		break;
	}
};

void CannonSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( deactivated_ )
	{
		graphics_->current_frame_x_ = 16;

		if ( collide_top_prev_ )
		{
			block_interact_ = false;
		}
	}
	else
	{
		if ( flash_time_ >= 0 )
		{
			graphics_->current_frame_x_ = 16;
			--flash_time_;
		}
		else
		{
			graphics_->current_frame_x_ = 0;
		}
	}

	if ( fellInBottomlessPit( lvmap ) )
	{
		kill();
	}
};

void CannonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( deactivated_ )
		{
			if ( their_collision.collideTop() && vy_ > 0 )
			{
				health.hurt();
			}
		}
		else
		{
			switch ( direction_y_ )
			{
				case ( Direction::Vertical::DOWN ):
					handleShooting( sprites, Direction::Simple::DOWN );
				break;

				case ( Direction::Vertical::UP ):
					handleShooting( sprites, Direction::Simple::UP );
				break;

				case ( Direction::Vertical::__NULL ):
					if ( them.xSubPixels() > hit_box_.right() + 32000 )
					{
						handleShooting( sprites, Direction::Simple::RIGHT );
					}
					else if ( them.rightSubPixels() < hit_box_.x - 32000 )
					{
						handleShooting( sprites, Direction::Simple::LEFT );
					}
				break;
			}

			if ( their_collision.collideAny() )
			{
				them.collideStopAny( their_collision );
			}
		}
	}
	else if ( them.hasType( SpriteType::HEROS_BULLET ) )
	{
		if ( !deactivated_ && !them.isDead() && their_collision.collideAny() )
		{
			hurt();
			them.kill();
		}
	}
};

int CannonSprite::shootTime() const
{
	return mezun::randInt( 140, 40 );
};

void CannonSprite::handleShooting( SpriteSystem& sprites, Direction::Simple dir )
{
	if ( shoot_counter_ >= shoot_time_ )
	{
		shoot_counter_ = 0;
		shoot_time_ = shootTime();
		sprites.spawnEnemyBullet( centerXPixels() - 2, yPixels() + 10, dir );
	}
	++shoot_counter_;
};

void CannonSprite::hurt()
{
	--hp_;
	Audio::playSound( Audio::SoundType::BOP );
	if ( hp_ <= 0 )
	{
		deactivated_ = true;
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
	else
	{
		flash_time_ = 8;
	}
};
