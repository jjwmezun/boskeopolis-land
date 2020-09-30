#include "collision.hpp"
#include "health.hpp"
#include "heat_beam_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static inline TimerSwitch timerType( const HeatBeamSprite::Type& type )
{
	return TimerSwitch( HeatBeamSprite::STATE_DURATION, HeatBeamSprite::TRANS_DURATION, type == HeatBeamSprite::Type::EVEN );
};

HeatBeamSprite::HeatBeamSprite( int x, int y, HeatBeamSprite::Type type )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "sprites/heatbeam.png", 0, 0, false, false, 0, -2, 0, 4, 0 ),
		x+2,
		y,
		12,
		80,
		{},
		0,
		0,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::RESET_OFFSCREEN_AND_AWAY,
		false,
		false
	),
	type_ ( type ),
	state_ ( State::OFF ),
	state_timer_ ( timerType( type ) ),
	timer_start_delay_ (),
	timer_speed_up_ ()
{
};

HeatBeamSprite::~HeatBeamSprite() {};

void HeatBeamSprite::customUpdate( LevelState& level_state )
{
	if ( state_timer_.inTrans() )
	{
		if ( state_timer_.transPercent() < .33 )
		{
			if ( state_timer_.switchStatus() )
			{
				graphics_->current_frame_x_ = 48;
			}
			else
			{
				graphics_->current_frame_x_ = 64;
			}
		}
		else if ( state_timer_.transPercent() < .66 )
		{
			graphics_->current_frame_x_ = 32;
		}
		else
		{
			if ( state_timer_.switchStatus() )
			{
				graphics_->current_frame_x_ = 64;
			}
			else
			{
				graphics_->current_frame_x_ = 48;
			}
		}
	}
	else if ( state_timer_.switchStatus() )
	{
		graphics_->current_frame_x_ = 16;
	}
	else
	{
		graphics_->current_frame_x_ = 0;
	}

	if ( state_timer_.on() )
	{
		state_timer_.update();
	}
	else
	{
		if ( timer_start_delay_.done() )
		{
			state_timer_.start();
		}
		else if ( timer_start_delay_.on() )
		{
			timer_start_delay_.update();
		}
		else
		{
			timer_start_delay_.start();
		}
	}

	if ( timer_speed_up_.done() )
	{
		state_timer_.speedUp();
		timer_speed_up_.stop();
	}
	else
	{
		timer_speed_up_.update();
	}
};

void HeatBeamSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( state_timer_.switchStatus() && !state_timer_.inTrans() )
		{
			if ( their_collision.collideAny() )
			{
				level_state.health().hurt();
			}
		}
	}
};
