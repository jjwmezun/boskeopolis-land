#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "render.hpp"

constexpr sdl2::SDLRect InventoryLevel::BG_DEST;
constexpr sdl2::SDLRect InventoryLevel::GEM_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::CLOCK_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::KEY_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::DIAMOND_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::MCGUFFIN_DEST;
constexpr sdl2::SDLRect InventoryLevel::MCGUFFIN_CROSS_DEST;

InventoryLevel::InventoryLevel()
:
	health_gfx_ ( VERTICAL_POS ),
	oxygen_meter_ ( VERTICAL_POS ),
	ticker_ ( VERTICAL_POS + 16 ),
	show_mcguffins_ ( false )
{};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	oxygen_meter_.update( health );
	Inventory::update();
	health_gfx_.update( health );
	ticker_.updateTicker();
};

void InventoryLevel::render( EventSystem& events )
{
	// BG
	Render::renderRect( BG_DEST, 1 );

	// GEMS
	gem_icon_gfx_.render( GEM_ICON_DEST, nullptr );
	Text::renderNumber( Inventory::fundsShown(), FUNDS_X, VERTICAL_POS, 5, Text::FontShade::DARK_GRAY );

	// HEALTH
	health_gfx_.render();

	// TIME
	Inventory::clock().render( CLOCK_X, VERTICAL_POS, nullptr, Text::FontShade::DARK_GRAY );
	clock_gfx_.render( CLOCK_ICON_DEST, nullptr );

	// KEY
	if ( events.hasKey() )
	{
		key_gfx_.render( KEY_ICON_DEST, nullptr );
	}

	/*
	// SWITCH
	if ( events.switchOn() )
	{
		switch_on_.render();
	}
	else
	{
		switch_off_.render();
	}*/

	// McGuffins
	if ( show_mcguffins_ )
	{
		mcguffins_gfx_.render( MCGUFFIN_DEST, nullptr );
		mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
		Text::renderNumber( Inventory::McGuffins(), MCGUFFIN_CROSS_DEST.x + 8, VERTICAL_POS, 1, Text::FontShade::DARK_GRAY );
	}

	// TICKER
	ticker_.render();

	// DIAMOND
	if ( Inventory::haveDiamond( Inventory::currentLevel() ) )
	{
		diamond_gfx_.render( DIAMOND_ICON_DEST, nullptr );
	}

	// OXYGEN
	oxygen_meter_.render();
};