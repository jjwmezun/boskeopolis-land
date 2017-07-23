



// Name
//===================================
//
// HeatBeamSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "heat_beam_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    HeatBeamSprite::HeatBeamSprite( int x, int y, Type type )
    :
        Sprite
		(
			std::make_unique<SpriteGraphics> ( "sprites/heatbeam.png", 0, 0, false, false, 0, false, -2, 0, 4, 0 ),
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
		timer_start_delay_ ( DELAY_DURATION ),
		timer_speed_up_ ( FRAMES_TILL_SPEED_UP )
    {
	};

    HeatBeamSprite::~HeatBeamSprite() {};

    void HeatBeamSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
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

    void HeatBeamSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( state_timer_.switchStatus() && !state_timer_.inTrans() )
			{
				if ( their_collision.collideAny() )
				{
					them.hurt();
				}
			}
		}
    };

	TimerSwitch HeatBeamSprite::timerType( const Type& type ) const
	{
		bool starts_on = false;
		
		if ( type == Type::EVEN )
		{
			starts_on = true;
		}
		
		return TimerSwitch( STATE_DURATION, TRANS_DURATION, starts_on );
	};