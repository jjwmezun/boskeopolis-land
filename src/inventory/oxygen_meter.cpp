#include "inventory.hpp"
#include "main.hpp"
#include "oxygen_meter.hpp"
#include "render.hpp"
#include "health.hpp"

OxygenMeter::OxygenMeter( int y )
:		
	gfx_left_bar_ ( "tilesets/universal.png", 0, 16 ),
	gfx_middle_bar_ ( "tilesets/universal.png", 8, 16 ),
	gfx_right_bar_ ( "tilesets/universal.png", 0, 16, true ),
	meter_bar_
	({
		X_PIXELS,
		y,
		width_pixels(),
		HEIGHT_PIXELS
	}),
	show_ ( false ),
	color_ ( 5 )
{};


void OxygenMeter::update( const Health& health )
{
	const double percent = health.oxygenPercent();

	if ( health.inWater() || percent < .998 )
	{
		show_ = true;
		meter_bar_.w = round( width_pixels() * percent );

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
		if ( Main::nextFrame( 2 ) )
		{
			color_ = 2;
		}
	}
};

void OxygenMeter::render() const
{
	if ( show_ )
	{
		renderMeter();
		renderShell();
	}
};

void OxygenMeter::renderMeter() const
{
	Render::renderRect( meter_bar_, color_ );
};

void OxygenMeter::renderShell() const
{	
	for ( int i = 0; i < width_mini_blocks(); ++i )
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
			gfx_left_bar_.render( r );
		}
		else if ( i == last_block() )
		{
			gfx_right_bar_.render( r );
		}
		else // is a center block
		{
			gfx_middle_bar_.render( r );
		}

	}
};

int OxygenMeter::width_mini_blocks() const
{
	return ( Inventory::haveOxygenUpgrade() ) ? BASE_WIDTH_UPGRADED : BASE_WIDTH;
};

int OxygenMeter::width_pixels() const
{
	return Unit::MiniBlocksToPixels( width_mini_blocks() );
};

int OxygenMeter::last_block() const
{
	return width_mini_blocks() - 1;
};