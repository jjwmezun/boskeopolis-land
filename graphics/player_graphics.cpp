


// Name
//===================================
//
// PlayerGraphics
//


// DEPENDENCIES
//===================================

    #include "player_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PlayerGraphics::PlayerGraphics( std::string&& texture )
    :
        SpriteGraphics ( std::forward<std::string> ( texture ), 0, 0, false, false, 0, false, -1, -2, 2, 4 ),
		walk_counter_  ( 0, 3, 0, true ),
		climb_counter_ ( 0, 1, 0, true ),
		blink_counter_ ( 0, 11, 0, true ),
		swim_counter_  ( 0, 2, 0, true ),
		swim_timer_    ( NULL, false )
    {};

    PlayerGraphics::~PlayerGraphics() {};

    void PlayerGraphics::update( Sprite& sprite )
    {		
        if ( sprite.directionX() == Direction::Horizontal::RIGHT )
        {
            flip_x_ = true;
        }
        else
        {
            flip_x_ = false;
        }


        if ( sprite.onLadder() )
        {
            if ( sprite.isMoving() )
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
                    flip_x_ = !flip_x_;
                    break;
            }

            current_frame_x_ = 96;
            current_frame_y_ = 0;
        }
        else if ( sprite.isSlidingPrev() )
        {
            current_frame_x_ = 48;
            current_frame_y_ = 26;
        }
        else if ( sprite.isDucking() )
        {
            if ( animation_timer_.hit() )
            {
                ++blink_counter_;
            }

            animation_timer_.update();

            switch ( blink_counter_.value() )
            {
                case 3:
                    current_frame_x_ = 0;
                    current_frame_y_ = 32;
                    break;
                default:
                    current_frame_x_ = 64;
                    current_frame_y_ = 6;
            }
        }
        else if ( !sprite.onGroundPrev() )
        {
            current_frame_x_ = 48;
            current_frame_y_ = 0;
        }
        else if ( sprite.isMoving() )
        {
            if ( animation_timer_.hit() )
            {
                ++walk_counter_;
            }

            animation_timer_.update();

            switch ( walk_counter_.value() )
            {
                case 1:
                    current_frame_x_ = 0;
                    break;
                case 2:
                    current_frame_x_ = 32;
                    break;
                case 3:
                    current_frame_x_ = 0;
                    break;
                default:
                    current_frame_x_ = 16;
            }

            current_frame_y_ = 0;
        }
        else
        {
            if ( animation_timer_.hit() )
            {
                ++blink_counter_;
            }

            animation_timer_.update();

            switch ( blink_counter_.value() )
            {
                case 3:
                    current_frame_x_ = 112;
                    break;
                default:
                    if ( sprite.lookingUp() )
                        current_frame_x_ = 80;
                    else
                        current_frame_x_ = 0;
            }

            current_frame_y_ = 0;
        }
		
        if ( sprite.hasMovementType( SpriteMovement::Type::SWIMMING ) && !sprite.onGroundPrev() )
        {
			switch ( swim_counter_() )
			{
				case ( 0 ):			
					current_frame_x_ = 64;
					current_frame_y_ = 26;		
				break;
					
				case ( 1 ):			
					current_frame_x_ = 80;
					current_frame_y_ = 26;		
				break;
					
				case ( 2 ):			
					current_frame_x_ = 96;
					current_frame_y_ = 26;		
				break;
			}
			
			if ( swim_timer_.hit() )
			{
				++swim_counter_;
				
				if ( swim_counter_ != 0 )
				{
					swim_timer_.start();
				}
				else
				{
					swim_timer_.restart();
					swim_timer_.stop();
				}
			}
			
			if ( swim_timer_.on() )
			{
				swim_timer_.update();
			}
			
			if ( sprite.isJumpingPrev() )
			{
				if ( !swim_timer_.on() )
				{
					swim_timer_.start();
				}
			}
        }
		else
		{
			swim_timer_.restart();
			swim_timer_.stop();
			swim_counter_.reset();
		}

		if ( sprite.isDead() )
		{
			current_frame_x_ = 16;
			current_frame_y_ = 26;
		}
    };

