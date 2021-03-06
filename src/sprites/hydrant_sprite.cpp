#include "audio.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "hydrant_sprite.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

HydrantSprite::HydrantSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/hydrant.png", 0, 0, false, false, 0, -1, -1, 2, 2 ), x, y, 14, 14, {}, 100, 1500, 3000, 3000 )
{
	jump_lock_ = false;
};

HydrantSprite::~HydrantSprite() {};

void HydrantSprite::customUpdate( LevelState& level_state )
{
	if ( isDead() )
	{
		block_interact_ = false;
	}

	if ( awake_ && delay_.done() )
	{
		if ( onGround() )
		{
			jump();
		}
	}
	else if ( delay_.counter() > 24 )
	{
		graphics_->current_frame_x_ = 32;
	}
	else if ( delay_.counter() > 8 )
	{
		graphics_->current_frame_x_ = 16;
	}

	if ( delay_.counter() % 4 == 1 )
	{
		graphics_->current_frame_y_ = 16;
	}
	else
	{
		graphics_->current_frame_y_ = 0;
	}
};

void HydrantSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( awake_ )
		{
			if ( delay_.done() )
			{
				if ( them.xSubPixels() > rightSubPixels() )
					moveRight();
				else
					moveLeft();

				if ( their_collision.collideAny() && them.bottomSubPixels() < ySubPixels() + Unit::PixelsToSubPixels( 12 ) )
				{
					kill();
					them.bounce();
					level_state.inventory().bop();
					Audio::playSound( Audio::SoundType::BOP );
				}
				else if ( their_collision.collideAny() && !isDead() )
				{
					level_state.health().hurt();
				}
			}
			else
			{
				delay_.update();
			}
		}
		else
		{
			if
			(
				them.rightSubPixels() > xSubPixels() - Unit::BlocksToSubPixels( 5 ) &&
				them.xSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 5 ) &&
				them.bottomSubPixels() > ySubPixels() - Unit::BlocksToSubPixels( 3 ) &&
				them.ySubPixels() < bottomSubPixels() + Unit::BlocksToSubPixels( 1 )
			)
			{
				awake_ = true;
				delay_.start();
			}
		}

		if ( !delay_.done() )
		{
			them.collideStopAny( their_collision );
		}

	}
	else if ( them.hasType( Sprite::SpriteType::ENEMY ) )
	{
		collideStopAny( my_collision );
	}
};
