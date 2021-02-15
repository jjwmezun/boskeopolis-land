#include "inventory.hpp"
#include "main.hpp"
#include "oxygen_meter.hpp"
#include "render.hpp"
#include "health.hpp"

static constexpr int BASE_WIDTH = 8;
static constexpr int BASE_WIDTH_UPGRADED = 12;
static constexpr int HEIGHT_MINIBLOCKS = 1;
static constexpr int HEIGHT_PIXELS = Unit::MiniBlocksToPixels( HEIGHT_MINIBLOCKS );
static constexpr int X_PIXELS = Unit::MiniBlocksToPixels( 27 );
static constexpr int FIRST_BLOCK = 0;

static int widthMiniBlocks()
{
	return ( Inventory::haveOxygenUpgrade() ) ? BASE_WIDTH_UPGRADED : BASE_WIDTH;
};

static int widthPixels()
{
	return Unit::MiniBlocksToPixels( widthMiniBlocks() );
};

static int lastBlock()
{
	return widthMiniBlocks() - 1;
};

OxygenMeter::OxygenMeter( int right, int y )
:
	gfx_left_bar_ ( "tilesets/universal.png", 0, 16 ),
	gfx_middle_bar_ ( "tilesets/universal.png", 8, 16 ),
	gfx_right_bar_ ( "tilesets/universal.png", 0, 16, true ),
	meter_bar_
	({
		right - widthPixels(),
		y,
		widthPixels(),
		HEIGHT_PIXELS
	}),
	show_ ( false ),
	color_ ( 5 )
{};


void OxygenMeter::update( const Health& health )
{
	const double percent = health.oxygenPercent();

	if ( health.losingMeter() || percent < .998 )
	{
		show_ = true;
		meter_bar_.w = round( widthPixels() * percent );

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
	for ( int i = 0; i < widthMiniBlocks(); ++i )
	{

		const sdl2::SDLRect r =
		{
			meter_bar_.x + Unit::MiniBlocksToPixels( i ),
			meter_bar_.y,
			Unit::MiniBlocksToPixels( 1 ),
			Unit::MiniBlocksToPixels( 1 )
		};

		if ( i == FIRST_BLOCK )
		{
			gfx_left_bar_.render( r );
		}
		else if ( i == lastBlock() )
		{
			gfx_right_bar_.render( r );
		}
		else // is a center block
		{
			gfx_middle_bar_.render( r );
		}

	}
};
