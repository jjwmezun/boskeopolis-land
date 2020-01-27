#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "map.hpp"
#include "mezun_helpers.hpp"
#include "news_ticker.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

static constexpr int MISC_X = 185;
static constexpr int FLASHING_TIMER_SHADES_NUM = 12;
static constexpr WTextCharacter::Color FLASHING_TIMER_SHADES[ FLASHING_TIMER_SHADES_NUM ] =
{
	WTextCharacter::Color::BLACK,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::BLACK
};
static constexpr int FLASHING_TIMER_SPEED = 8;

InventoryLevel::InventoryLevel()
:
	health_gfx_ ( Y ),
	oxygen_meter_ ( Y ),
	ticker_ ( NewsTicker::make( Y + 8 ) ),
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 ),
	main_texture_ ( Unit::WINDOW_WIDTH_PIXELS, HEIGHT, 0, Y ),
	kill_count_ ( -1 ),
	showing_key_ ( false )
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
			updateTimerGraphics();
		}
	}
	if ( !showing_key_ && events.hasKey() )
	{
		updateKeyGraphics();
		showing_key_ = true;
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
	Text::renderNumber( Inventory::fundsShown(), FUNDS_X, Y, 5, WTextCharacter::Color::DARK_GRAY );

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
			Text::renderNumber( Inventory::McGuffins(), MCGUFFIN_CROSS_DEST.x + 8, Y, 1, WTextCharacter::Color::DARK_GRAY );
		}*/

		// Kill Count
		/*if ( kill_counter_ > -1 )
		{
			kills_gfx_.render( MCGUFFIN_DEST, nullptr );
			mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
			Text::renderNumber( kill_counter_, MCGUFFIN_CROSS_DEST.x + 8, Y, 1, WTextCharacter::Color::DARK_GRAY );
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
		WTextObj text( mezun::intToChar32String( Inventory::howManyBops() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
	else if ( Inventory::multipleGhostKills() )
	{
		WTextObj text( mezun::intToChar32String( Inventory::howManyGhostKills() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
};

void InventoryLevel::setShowMcGuffins()
{

};

void InventoryLevel::changeKillCounter( int count )
{
	kill_count_ = count;
	updateKillCountGraphics();
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
		Render::renderObject( "bg/level-select-characters.png", { 16, 40, 8, 8 }, { 10, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( Inventory::haveDiamond() )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 32, 8, 8 }, { 18, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	health_gfx_.render();
	renderPtsGraphics();
	renderTimerGraphics();
	if ( kill_count_ != -1 )
	{
		renderKillCountGraphics();
	}
	if ( showing_key_ )
	{
		renderKeyGraphics();
	}
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
	Render::renderRect( { CLOCK_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderTimerGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderTimerGraphics()
{
	WTextObj text{ Inventory::clock().getTimeString(), CLOCK_X, TOP_ROW_Y_RELATIVE, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] };
	text.render();
};

void InventoryLevel::updateKillCountGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderKillCountGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderKillCountGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 0, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ U"x" + mezun::intToChar32StringWithPadding( kill_count_, 2 ), MISC_X + 8, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevel::updateKeyGraphics()
{
	main_texture_.startDrawing();
	renderKeyGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderKeyGraphics()
{
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 }, 1 );
	Render::renderObject( "bg/level-select-characters.png", { 8, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
};