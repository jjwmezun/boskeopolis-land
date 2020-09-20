#include "event_system.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "shooter_player_sprite.hpp"
#include "sprite_graphics.hpp"
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
		2000,
		{ false, 9, 11, -8, 11 },
		{ 10, 0, -3, 5 }
	),
	is_shooting_ ( false ),
	is_shooting_up_ ( false ),
	shoot_delay_count_ ( 0 ),
	is_shooting_count_ ( 0 ),
	walk_counter_  (),
	climb_counter_ (),
	animation_timer_ ()
{
};

ShooterPlayerSprite::~ShooterPlayerSprite() {};

void ShooterPlayerSprite::customUpdate( LevelState& level_state )
{
	heroActions( level_state );

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
		SpriteSystem& sprites = level_state.sprites();
		if ( isLookingUp() )
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

	if ( on_ladder_ )
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
		if ( isLookingUp() )
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

void ShooterPlayerSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	graphics_->current_frame_x_ = 208;
	graphics_->current_frame_y_ = 0;
	defaultDeathAction( camera );
	events.playDeathSoundIfNotAlreadyPlaying();
};
