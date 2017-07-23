#include "cactooie_sprite.h"
#include "sprite_graphics.h"
#include "sprite_system.h"

CactooieSprite::CactooieSprite( int x, int y )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "sprites/cactooie.png", 0, 0, false, false, 0, false, -4, -4, 8, 8 ),
		x,
		y,
		16,
		16,
		{ SpriteType::ENEMY },
		500,
		500,
		0,
		0,
		Direction::Horizontal::LEFT,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::GROUNDED
	),
	state_ ( CactooieState::STILL ),
	still_delay_ ( 32, false ),
	spitting_delay_ ( 48, false )
{};

CactooieSprite::~CactooieSprite() {};

void CactooieSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	switch ( state_ )
	{
		case ( CactooieState::STILL ):
			if ( still_delay_.done() )
			{
				if ( mezun::testRandomWithinPercentage( 40 ) )
				{
					state_ = CactooieState::SPITTING;
				}
				else
				{
					state_ = CactooieState::ROLLING;
				}
					
				still_delay_.stop();
			}
			else if ( still_delay_.on() )
			{
				if ( still_delay_.countPercent() == .5 )
				{
					direction_x_ = Direction::randomHorizontal( false );
				}
				
				still_delay_.update();
			}
			else
			{
				still_delay_.start();
			}
			fullStopX();
		break;
			
		case ( CactooieState::ROLLING ):
			if ( direction_x_ == Direction::Horizontal::RIGHT )
			{
				moveRight();
			}
			else
			{
				moveLeft();
			}

			if ( collidedRight() )
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}
			else if ( collidedLeft() )
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}
			
			if ( rollFinished() )
			{
				state_ = CactooieState::STILL;
			}
		break;
			
		case ( CactooieState::SPITTING ):
			if ( spitting_delay_.done() )
			{
				state_ = CactooieState::STILL;
				spitting_delay_.stop();
			}
			else if ( spitting_delay_.on() )
			{
				if ( spitting_delay_.countPercent() == .5 )
				{
					sprites.spawnCactooieSpine( xPixels(), centerYPixels() - 4, direction_x_ );
				}
				
				spitting_delay_.update();
			}
			else
			{
				spitting_delay_.start();
			}
		break;
	};
	
	updateGraphics();
};

void CactooieSprite::updateGraphics()
{
	switch ( state_ )
	{
		case ( CactooieState::STILL ):
			graphics_->current_frame_x_ = 0;
			graphics_->rotation_ = 0;
		break;
			
		case ( CactooieState::ROLLING ):
			graphics_->current_frame_x_ = 24;
			
			if ( direction_x_ == Direction::Horizontal::RIGHT )
			{
				graphics_->rotation_ += 5;
			}
			else
			{
				graphics_->rotation_ -= 5;
			}
		break;
			
		case ( CactooieState::SPITTING ):
			graphics_->current_frame_x_ = 48;
			graphics_->rotation_ = 0;
		break;
	};
	
			
	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		graphics_->flip_x_ = true;
	}
	else
	{
		graphics_->flip_x_ = false;
	}
};

bool CactooieSprite::rollFinished() const
{
	return abs( graphics_->rotation_ ) >= 360;
};

void CactooieSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
};
