



// Name
//===================================
//
// OverworldPlayerGraphics
//


// DEPENDENCIES
//===================================

    #include "overworld_player_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    OverworldPlayerGraphics::OverworldPlayerGraphics()
    :
        SpriteGraphics ( Graphics::SpriteSheet::LVSPRITES_AUTUMN_OVERWORLD, 0, 0, false, false, 0, false, -2, -1, 3, 2, 8 )
    {};

    OverworldPlayerGraphics::~OverworldPlayerGraphics() {};

    void OverworldPlayerGraphics::update( Sprite& sprite )
    {
		if ( animation_timer_.hit() )
		{
			if ( current_frame_x_ == 0 )
			{
				current_frame_x_ = 15;
			}
			else
			{
				current_frame_x_ = 0;
			}
			
			animation_timer_.stop();
		}
		else if ( animation_timer_.on() )
		{
			animation_timer_.update();
		}
		else
		{
			animation_timer_.start();
		}
    };

