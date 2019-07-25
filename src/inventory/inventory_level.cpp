#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "map.hpp"
#include "news_ticker.hpp"
#include "render.hpp"
#include "sprite.hpp"

constexpr sdl2::SDLRect InventoryLevel::BG_DEST;
constexpr sdl2::SDLRect InventoryLevel::VICTORY_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::DIAMOND_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::GEM_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::CLOCK_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::KEY_ICON_DEST;
constexpr sdl2::SDLRect InventoryLevel::MCGUFFIN_DEST;
constexpr sdl2::SDLRect InventoryLevel::MCGUFFIN_CROSS_DEST;

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
	ticker_ ( NewsTicker::make( Y + 16 ) ),
	show_mcguffins_ ( false ),
	kill_counter_ ( -1 ),
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 )
{};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	oxygen_meter_.update( health );
	Inventory::update();
	health_gfx_.update( health );
	ticker_.update();

	if ( Inventory::clock().lowOnTime() )
	{
		++flashing_timer_;

		if ( flashing_timer_ >= Inventory::clock().timeRemaining() )
		{
			++flashing_time_shade_;
			flashing_timer_ = 0;

			if ( flashing_time_shade_ >= FLASHING_TIMER_SHADES_NUM )
			{
				flashing_time_shade_ = 0;
			}
		}
	}
};

void InventoryLevel::render( const EventSystem& events, const Sprite& hero, const Camera& camera, const Map& lvmap )
{
	// BG
	Render::renderRect( BG_DEST, lvmap.ui_bg_color_ );

	// VICTORY
	if ( Inventory::victory() )
	{
		victory_gfx_.current_frame_x_ = HAVE_X;
	}
	else
	{
		victory_gfx_.current_frame_x_ = DONT_HAVE_X;
	}
	victory_gfx_.render( VICTORY_ICON_DEST, nullptr );

	// DIAMOND
	if ( Inventory::haveDiamond() )
	{
		diamond_gfx_.current_frame_x_ = HAVE_X;
	}
	else
	{
		diamond_gfx_.current_frame_x_ = DONT_HAVE_X;
	}
	diamond_gfx_.render( DIAMOND_ICON_DEST, nullptr );

	// GEMS
	gem_icon_gfx_.render( GEM_ICON_DEST, nullptr );
	Text::renderNumber( Inventory::fundsShown(), FUNDS_X, Y, 5, Text::FontColor::DARK_GRAY );

	// HEALTH
	health_gfx_.render();

	// TIME
	Inventory::clock().render( CLOCK_X, Y, nullptr, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] );
	clock_gfx_.render( CLOCK_ICON_DEST, nullptr );

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

		// McGuffins
		if ( show_mcguffins_ )
		{
			mcguffins_gfx_.render( MCGUFFIN_DEST, nullptr );
			mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
			Text::renderNumber( Inventory::McGuffins(), MCGUFFIN_CROSS_DEST.x + 8, Y, 1, Text::FontColor::DARK_GRAY );
		}

		// Kill Count
		if ( kill_counter_ > -1 )
		{
			kills_gfx_.render( MCGUFFIN_DEST, nullptr );
			mcguffins_cross_.render( MCGUFFIN_CROSS_DEST, nullptr );
			Text::renderNumber( kill_counter_, MCGUFFIN_CROSS_DEST.x + 8, Y, 1, Text::FontColor::DARK_GRAY );
		}

	// OXYGEN
	oxygen_meter_.render();

	// TICKER
	if ( !events.hide_ticker_ )
	{
		ticker_.render();
	}

	// BOPS
	if ( Inventory::bopsMultiplier() )
	{
		Text::renderNumber( Inventory::howManyBops(), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), 1, Text::FontColor::DARK_GRAY );
	}
};
