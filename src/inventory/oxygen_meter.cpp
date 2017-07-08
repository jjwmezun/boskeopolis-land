


// Name
//===================================
//
// OxygenMeter
//


// DEPENDENCIES
//===================================

	#include "game.h"
	#include "graphics.h"
    #include "oxygen_meter.h"
	#include "sprite.h"


// STATIC PROPERTIES
//===================================



// METHODS
//===================================

	OxygenMeter::OxygenMeter( int y )
	:		
		gfx_left_bar_ ( "tilesets/universal.png", 0, 16 ),
		gfx_middle_bar_ ( "tilesets/universal.png", 8, 16 ),
		gfx_right_bar_ ( "tilesets/universal.png", 0, 16, true ),
		meter_bar_
		({
			X_PIXELS,
			y,
			WIDTH_PIXELS,
			HEIGHT_PIXELS
		}),
		show_ ( false ),
		color_ ( 5 )
	{};


	void OxygenMeter::update( Sprite& hero )
	{
        if ( hero.in_water_ )
        {
            show_ = true;
			const double percent = hero.status().oxygenPercent();
            meter_bar_.w = round( WIDTH_PIXELS * percent );
			
			color( percent );
			
        }
        else
        {
            show_ = false;
        }
	};

	void OxygenMeter::color( double percent )
	{
		color_ = 5;
		
		if ( percent < .25 )
		{
			if ( Game::nextFrame( 2 ) )
			{
				color_ = 2;
			}
		}
	};

	void OxygenMeter::render( Graphics& graphics ) const
	{
        if ( show_ )
        {
			renderMeter( graphics );
			renderShell( graphics );
		}
	};

	void OxygenMeter::renderMeter( Graphics& graphics ) const
	{
		graphics.renderRect( meter_bar_, color_ );
	};

	void OxygenMeter::renderShell( Graphics& graphics ) const
	{	
		for ( int i = 0; i < WIDTH_MINIBLOCKS; ++i )
		{
						
			const sdl2::SDLRect r =
			{
				X_PIXELS + Unit::MiniBlocksToPixels( i ),
				meter_bar_.y,
				Unit::MiniBlocksToPixels( 1 ),
				Unit::MiniBlocksToPixels( 1 )
			};
			
			if ( i == FIRST_BLOCK )
			{
				gfx_left_bar_.render( graphics, r );
			}
			else if ( i == LAST_BLOCK )
			{
				gfx_right_bar_.render( graphics, r );
			}
			else // is a center block
			{
				gfx_middle_bar_.render( graphics, r );
			}
			
		}
	};