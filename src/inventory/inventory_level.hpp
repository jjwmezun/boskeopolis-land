#pragma once

class EventSystem;
class Health;

#include "inventory_level_health.hpp"
#include "mezun_sdl2.hpp"
#include "news_ticker.hpp"
#include "oxygen_meter.hpp"
#include "sprite_graphics.hpp"
#include "text.hpp"

class InventoryLevel
{
	public:
		InventoryLevel();

		void update( EventSystem& events, const Health& health );
		void render( EventSystem& events );

		bool show_mcguffins_;
		int kill_counter_;

	private:
		static constexpr int Y = Unit::WINDOW_HEIGHT_PIXELS - 24;
		static constexpr int VICTORY_X = Unit::MiniBlocksToPixels( 1 );
		static constexpr int DIAMOND_X = Unit::MiniBlocksToPixels( 2 );
		static constexpr int FUNDS_ICON_X = Unit::MiniBlocksToPixels( 4 );
		static constexpr int FUNDS_X = FUNDS_ICON_X + 8;
		static constexpr int CLOCK_ICON_X = Unit::MiniBlocksToPixels( 17 );
		static constexpr int CLOCK_X = CLOCK_ICON_X + 8;
		static constexpr int MISC_X = Unit::MiniBlocksToPixels( 23 );

		static constexpr sdl2::SDLRect BG_DEST = { 0, Unit::WINDOW_HEIGHT_PIXELS - 32, Unit::WINDOW_WIDTH_PIXELS, 32 };
		static constexpr sdl2::SDLRect VICTORY_ICON_DEST = { VICTORY_X, Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect DIAMOND_ICON_DEST = { DIAMOND_X, Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect GEM_ICON_DEST = { FUNDS_ICON_X, Y, 8, 8 };
		static constexpr sdl2::SDLRect CLOCK_ICON_DEST = { CLOCK_ICON_X, Y, 8, 8 };

		static constexpr sdl2::SDLRect KEY_ICON_DEST = { MISC_X, Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect MCGUFFIN_DEST = { MISC_X, Y, 8, 8 };
		static constexpr sdl2::SDLRect MCGUFFIN_CROSS_DEST = { MISC_X + 8, Y, 8, 8 };

		static constexpr int HAVE_X = 48;
		static constexpr int DONT_HAVE_X = 56;

		SpriteGraphics victory_gfx_ = SpriteGraphics( "tilesets/universal.png", DONT_HAVE_X, 8 );
		SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", DONT_HAVE_X, 0 );
		const SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 48 );
		const SpriteGraphics clock_gfx_ = SpriteGraphics( "tilesets/universal.png", 96, 48 );

		const SpriteGraphics key_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 8 );
		const SpriteGraphics mcguffins_gfx_ = SpriteGraphics( "tilesets/universal.png", 0, 24 );
		const SpriteGraphics kills_gfx_ = SpriteGraphics( "tilesets/universal.png", 8, 24 );
		const SpriteGraphics mcguffins_cross_ = SpriteGraphics( "charset.png", 8, 40 );
		const Text switch_on_ = { "ON", MISC_X, Y, Text::FontShade::DARK_GRAY };
		const Text switch_off_ = { "OFF", MISC_X, Y, Text::FontShade::DARK_GRAY };

		InventoryLevelHealth health_gfx_;
		OxygenMeter oxygen_meter_;
		NewsTicker ticker_;
};