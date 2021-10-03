#include "reticle_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

ReticleSprite::ReticleSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/reticle.png" ), x, y, 32, 32, {}, 4000, 4000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	state_ ( ReticleState::CHASING ),
	timer_ ( 0 )
{
	layer_ = Unit::Layer::SPRITES_2;
};

ReticleSprite::~ReticleSprite() {};

void ReticleSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( ReticleState::CHASING ):
		{
			if ( timer_ == 60 )
			{
				state_ = ReticleState::STOPPING;
				timer_ = 0;
				graphics_->current_frame_x_ = 32;
				fullStopX();
				fullStopY();
			}
			else
			{
				++timer_;
			}
		}
		break;
		case ( ReticleState::STOPPING ):
		{
			if ( timer_ == 24 )
			{
				state_ = ReticleState::SHOOTING;
				timer_ = 0;
				graphics_->current_frame_x_ = 64;
			}
			else
			{
				++timer_;
			}
		}
		break;
		case ( ReticleState::SHOOTING ):
		{
			if ( timer_ == 40 )
			{
				state_ = ReticleState::CHASING;
				timer_ = 0;
				graphics_->current_frame_x_ = 0;
			}
			else
			{
				++timer_;
			}
		}
		break;
	}
};

void ReticleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		switch ( state_ )
		{
			case ( ReticleState::CHASING ):
			{
				if ( them.centerXSubPixels() > centerXSubPixels() )
				{
					moveRight();
				}
				else if ( them.centerXSubPixels() < centerXSubPixels() )
				{
					moveLeft();
				}

				if ( them.centerYSubPixels() > centerYSubPixels() )
				{
					moveDown();
				}
				else if ( them.centerYSubPixels() < centerYSubPixels() )
				{
					moveUp();
				}
			}
			break;
			case ( ReticleState::SHOOTING ):
			{
				if ( their_collision.collideAny() )
				{
					level_state.health().hurt();
				}
			}
			break;
		}
	}
};
