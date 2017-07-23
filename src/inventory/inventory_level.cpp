#include "event_system.hpp"
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
	inventory_ ( {} ),
	health_ ( VERTICAL_POS ),
	clock_ ( {} ),
	oxygen_meter_ ( VERTICAL_POS ),
	ticker_ ( VERTICAL_POS + 16 ),
	show_mcguffins_ ( false )
{};

InventoryLevel::InventoryLevel( const Inventory& c )
:
	inventory_ ( c ),
	health_ ( VERTICAL_POS ),
	clock_ ( {} ),
	oxygen_meter_ ( VERTICAL_POS ),
	ticker_ ( VERTICAL_POS + 16 ),
	show_mcguffins_ ( false )
{};

InventoryLevel::InventoryLevel( Inventory&& m )
:
	inventory_ ( m ),
	health_ ( VERTICAL_POS ),
	clock_ ( {} ),
	oxygen_meter_ ( VERTICAL_POS ),
	ticker_ ( VERTICAL_POS + 16 ),
	show_mcguffins_ ( false )
{};

void InventoryLevel::init()
{
	clock_.reset();
	inventory_.funds_ = 0;
};

void InventoryLevel::update( EventSystem& events, Sprite& hero )
{
	oxygen_meter_.update( hero );
	inventory_.update();
	health_.update( hero );
	clock_.update();
	ticker_.updateTicker();
};

void InventoryLevel::render( int level, EventSystem& events )
{
	// BG
	Render::renderRect( BG_DEST, 1 );

	// GEMS
	gem_icon_gfx_.render( GEM_ICON_DEST, nullptr );
	Text::renderNumber( inventory_.fundsShown(), FUNDS_X, VERTICAL_POS, 5, Text::FontShade::DARK_GRAY );

	// HEALTH
	health_.render();

	// TIME
	clock_.render( CLOCK_X, VERTICAL_POS, nullptr, Text::FontShade::DARK_GRAY );
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
		Text::renderNumber( inventory_.McGuffins(), MCGUFFIN_CROSS_DEST.x + 8, VERTICAL_POS, 1, Text::FontShade::DARK_GRAY );
	}

	// TICKER
	ticker_.render();

	// DIAMOND
	if ( inventory_.haveDiamond( level ) )
	{
		diamond_gfx_.render( DIAMOND_ICON_DEST, nullptr );
	}

	// OXYGEN
	oxygen_meter_.render();
};

void InventoryLevel::addFunds( int n )
{
	inventory_.funds_ += n;
};

void InventoryLevel::loseFunds( int n )
{
	inventory_.funds_ -= n;
};

int InventoryLevel::funds() const
{
	return inventory_.funds_();
};

int InventoryLevel::clockTime() const
{
	return clock_.totalSeconds();
};

bool InventoryLevel::haveDiamond( int level ) const
{
	return inventory_.haveDiamond( level );
};

void InventoryLevel::getDiamond( int level )
{
	inventory_.getDiamond( level );
};

Clock& InventoryLevel::clock()
{
	return clock_;
};

void InventoryLevel::setFunds( int n )
{
	inventory_.funds_ = n;
};

const Inventory& InventoryLevel::inventory() const
{
	return inventory_;
};

void InventoryLevel::won( int level )
{
	inventory_.victories_.at( level ) = true;
	inventory_.setGemScore( level );
	inventory_.setTimeScore( level, clockTime() );

	inventory_.total_funds_ += inventory_.funds_;
	inventory_.funds_ = 0;
	inventory_.save();
};

void InventoryLevel::failed()
{
	inventory_.total_funds_ -= inventory_.PRICE_OF_DEATH;
	inventory_.save();
};

void InventoryLevel::quit( int level )
{
	if ( !inventory_.victories_.at( level ) )
	{
		failed();
	}
};

void InventoryLevel::registerBeenToLevel( int level )
{
	inventory_.registerBeenToLevel( level );
};

void InventoryLevel::addMcGuffin() { inventory_.addMcGuffin(); };
int InventoryLevel::McGuffins() const { return inventory_.McGuffins(); };