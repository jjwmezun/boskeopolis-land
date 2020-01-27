#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "map.hpp"
#include "news_ticker.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

static constexpr int FLASHING_TIMER_SHADES_NUM = 8;
static constexpr Text::FontColor FLASHING_TIMER_SHADES[ FLASHING_TIMER_SHADES_NUM ] =
{
	Text::FontColor::DARK_GRAY,
	Text::FontColor::DARK_MID_GRAY,
	Text::FontColor::LIGHT_MID_GRAY,
	Text::FontColor::LIGHT_GRAY,
	Text::FontColor::LIGHT_GRAY,
	Text::FontColor::LIGHT_MID_GRAY,
	Text::FontColor::DARK_MID_GRAY,
	Text::FontColor::DARK_GRAY
};
static constexpr int FLASHING_TIMER_SPEED = 8;

InventoryLevel::InventoryLevel()
:
	health_gfx_ ( Y ),
	oxygen_meter_ ( Y ),
	ticker_ ( NewsTicker::make( Y + 8 ) ),
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 ),
	main_texture_ ( Unit::WINDOW_WIDTH_PIXELS, HEIGHT, 0, Y )
{};

InventoryLevel::~InventoryLevel()
{
	main_texture_.destroy();
};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	oxygen_meter_.update( health );
	if ( Inventory::updateLevelFunds() )
	{
		updatePtsGraphics();
	}
	if ( Inventory::clock().update() )
	{
		updateTimerGraphics();
	}
	if ( health_gfx_.update( health ) )
	{
		updateHealthGraphics();
	}
	ticker_.update();
	if ( Inventory::clock().lowOnTime() )
	{
		++flashing_timer_;
		if ( flashing_timer_ >= Inventory::clock().timeRemaining() )
		{
			flashing_timer_ = 0;
			++flashing_time_shade_;
			if ( flashing_time_shade_ == FLASHING_TIMER_SHADES_NUM )
			{
				flashing_time_shade_ = 0;
			}
		}
	}
};

void InventoryLevel::init()
{
	main_texture_.init();
	forceRerender();
};

void InventoryLevel::render( const EventSystem& events, const Sprite& hero, const Camera& camera, const Map& lvmap )
{
	main_texture_.render();

/*
	// GEMS
	Text::renderNumber( Inventory::fundsShown(), FUNDS_X, Y, 5, Text::FontColor::DARK_GRAY );

	// TIME
	Inventory::clock().render( CLOCK_X, Y, nullptr, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] );
	clock_gfx_.render( CLOCK_ICON_DEST, nullptr );
/*
	// MISC
		// KEY
		if ( events.hasKey() )
		{
			key_gfx_.render( KEY_ICON_DEST, nullptr );
		}

		// SWITCH
		if ( lvmap.show_on_off_ )
		{
			if ( events.switchOn() )
			{
				switch_on_.render();
			}
			else
			{
				switch_off_.render();
			}
		}

		/*
		// McGuffins
		//if ( show_mcguffins_ )
		{
			mcguffins_gfx_.render( MCGUFFIN_DEST, nullptr );
			mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
			Text::renderNumber( Inventory::McGuffins(), MCGUFFIN_CROSS_DEST.x + 8, Y, 1, Text::FontColor::DARK_GRAY );
		}*/

		// Kill Count
		/*if ( kill_counter_ > -1 )
		{
			kills_gfx_.render( MCGUFFIN_DEST, nullptr );
			mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
			Text::renderNumber( kill_counter_, MCGUFFIN_CROSS_DEST.x + 8, Y, 1, Text::FontColor::DARK_GRAY );
		}*/

	// OXYGEN
	oxygen_meter_.render();

	// TICKER
	if ( events.showBossUI() )
	{
		events.renderBossUI();
	}
	else
	{
		//ticker_.render();
	}

	// BOPS
	if ( Inventory::bopsMultiplier() )
	{
		Text::renderNumber( Inventory::howManyBops(), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), 1, Text::FontColor::DARK_GRAY );
	}
	else if ( Inventory::multipleGhostKills() )
	{
		Text::renderNumber( Inventory::howManyGhostKills(), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), 1, Text::FontColor::DARK_GRAY );
	}
};

void InventoryLevel::setShowMcGuffins()
{

};

void InventoryLevel::setKillCounter( int count )
{

};

void InventoryLevel::updateHealthGraphics()
{
	main_texture_.startDrawing();
	health_gfx_.render();
	main_texture_.endDrawing();
};

void InventoryLevel::forceRerender()
{
	main_texture_.startDrawing();
	Render::clearScreenTransparency();
	Render::renderObject( "bg/level-inventory-frame.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT } );
	if ( Inventory::victory() )
	{
		Render::renderObject( "bg/level-inventory-frame.png", { 16, 40, 8, 8 }, { 10, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( Inventory::haveDiamond() )
	{
		Render::renderObject( "bg/level-inventory-frame.png", { 16, 32, 8, 8 }, { 18, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	health_gfx_.render();
	renderPtsGraphics();
	renderTimerGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::updatePtsGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { 42, TOP_ROW_Y_RELATIVE, 8 * 5, 8 }, 1 );
	renderPtsGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderPtsGraphics()
{
	WTextObj text{ Inventory::fundsString(), 42, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevel::updateTimerGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { 146, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderTimerGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderTimerGraphics()
{
	WTextObj text{ Inventory::clock().getTimeString(), 146, TOP_ROW_Y_RELATIVE };
	text.render();
};