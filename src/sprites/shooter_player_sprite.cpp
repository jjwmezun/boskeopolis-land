#include "input.hpp"
#include "shooter_player_sprite.hpp"
#include "sprite_system.hpp"

ShooterPlayerSprite::ShooterPlayerSprite( int x, int y )
:
	PlayerSprite
	(
		x,
		y,
		1000,
		6000,
		std::unique_ptr<InputComponent> ( new InputComponentPlayer() ),
		std::make_unique<SpriteGraphics> ( "sprites/cowgirl_autumn.png", 0, 0, false, false, 0, false, -8, -3, 12, 5 ),
		SpriteType::HERO,
		160,
		2000
	),
	is_shooting_ ( false ),
	is_shooting_up_ ( false ),
	shoot_delay_count_ ( 0 ),
	is_shooting_count_ ( 0 ),
	animation_timer_ (),
	walk_counter_  ( 0, 3, 0, true ),
	climb_counter_ ( 0, 1, 0, true )
{
	direction_x_ = Direction::Horizontal::RIGHT;
};

ShooterPlayerSprite::~ShooterPlayerSprite() {};

void ShooterPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	heroActions( camera, lvmap, events, sprites, blocks, health );

	if ( is_shooting_ )
	{
		if ( is_shooting_count_ >= 8 )
		{
			is_shooting_ = false;
			is_shooting_up_ = false;
			is_shooting_count_ = 0;
		}
		else
		{
			++is_shooting_count_;
		}
	}

	if ( shoot_delay_count_ > 0 )
	{
		--shoot_delay_count_;
	}
	
	if ( shoot_delay_count_ == 0 && Input::pressed( Input::Action::RUN ) )
	{
		if ( lookingUp() )
		{
			sprites.spawnHeroBullet( centerXPixels(), yPixels() + 2, Direction::Simple::UP );	
			is_shooting_up_ = true;
		}
		else
		{
			sprites.spawnHeroBullet( centerXPixels(), yPixels() + 2, Direction::horizontalToSimple( direction_x_ ) );
		}
		shoot_delay_count_ = 16;
		is_shooting_ = true;
	}

	updateGFX();
};

void ShooterPlayerSprite::updateGFX()
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
			graphics_->x_adjustment_ = -2;
		break;

		default:
			graphics_->flip_x_ = false;
			graphics_->x_adjustment_ = -8;
		break;
	}

	if ( onLadder() )
	{
		if ( isMoving() )
		{
			if ( animation_timer_.hit() )
			{
				++climb_counter_;
			}

			animation_timer_.update();
		}

		
		switch ( climb_counter_.value() )
		{
			case 1:
				graphics_->current_frame_x_ = 156;
			break;

			default:
				graphics_->current_frame_x_ = 130;
			break;
		}
	}
	else if ( isDucking() )
	{
		graphics_->current_frame_x_ = 104;
	}
	else if ( !onGroundPrev() )
	{
		graphics_->current_frame_x_ = 78;
	}
	else if ( isMoving() )
	{
		if ( animation_timer_.hit() )
		{
			++walk_counter_;
		}

		animation_timer_.update();

		switch ( walk_counter_.value() )
		{
			case 1:
			case 3:
				if ( is_shooting_up_ )
				{
					graphics_->current_frame_x_ = 182;
				}
				else
				{
					graphics_->current_frame_x_ = 0;
				}
			break;
			case 2:
				if ( is_shooting_up_ )
				{
					graphics_->current_frame_x_ = 234;
				}
				else
				{
					graphics_->current_frame_x_ = 52;
				}
				break;
			default:
				if ( is_shooting_up_ )
				{
					graphics_->current_frame_x_ = 208;
				}
				else
				{
					graphics_->current_frame_x_ = 26;
				}
		}
	}
	else
	{		
		if ( lookingUp() )
		{
			graphics_->current_frame_x_ = 182;
		}
		else
		{
			graphics_->current_frame_x_ = 0;
		}
	}
	
	if ( is_shooting_ )
	{
		graphics_->current_frame_y_ = 28;
	}
	else
	{
		graphics_->current_frame_y_ = 0;
	}
};

void ShooterPlayerSprite::duck()
{
	// Can continue ducking while in air, but only start duck on ground.
	if ( isDucking() || onGround() )
	{
		// Hacky way to make player warp to the right position after height changes.
		if ( !isDucking() )
		{
			hit_box_.y += Unit::PixelsToSubPixels( 9 );
			graphics_->y_adjustment_ = -8;
			graphics_->h_adjustment_ = 11;
		}

		is_ducking_ = true;
		hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( 11 );
	}
};

void ShooterPlayerSprite::unduck()
{
	// Hacky way to keep player from falling through ground after gaining height from unducking.
	if ( isDucking() )
	{
		hit_box_.y -= Unit::PixelsToSubPixels( 10 );
		graphics_->y_adjustment_ = -3;
		graphics_->h_adjustment_ = 5;
	}

	is_ducking_ = false;
	hit_box_.h = original_hit_box_.h;
};

void ShooterPlayerSprite::deathAction( Camera& camera )
{
	graphics_->current_frame_x_ = 208;
	graphics_->current_frame_y_ = 0;
	defaultDeathAction( camera );
};