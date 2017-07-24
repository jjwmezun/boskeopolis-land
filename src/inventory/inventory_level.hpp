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

	private:
		static constexpr int FUNDS_X = 16;
		static constexpr int CLOCK_X = 120;
		static constexpr int VERTICAL_POS = Unit::WINDOW_HEIGHT_PIXELS - 24;
		static constexpr sdl2::SDLRect BG_DEST = { 0, Unit::WINDOW_HEIGHT_PIXELS - 32, Unit::WINDOW_WIDTH_PIXELS, 32 };
		static constexpr sdl2::SDLRect GEM_ICON_DEST = { 8, VERTICAL_POS, 8, 8 };
		static constexpr sdl2::SDLRect CLOCK_ICON_DEST = { 112, VERTICAL_POS, 8, 8 };
		static constexpr sdl2::SDLRect KEY_ICON_DEST = { 176, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect DIAMOND_ICON_DEST = { 160, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr int MCGUFFIN_X = DIAMOND_ICON_DEST.x + 16;
		static constexpr sdl2::SDLRect MCGUFFIN_DEST = { MCGUFFIN_X, VERTICAL_POS, 8, 8 };
		static constexpr sdl2::SDLRect MCGUFFIN_CROSS_DEST = { MCGUFFIN_X + 8, VERTICAL_POS, 8, 8 };

		const SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 48 );
		const SpriteGraphics clock_gfx_ = SpriteGraphics( "tilesets/universal.png", 40, 0 );
		const SpriteGraphics key_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 8 );
		const SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 0 );
		const SpriteGraphics mcguffins_gfx_ = SpriteGraphics( "tilesets/universal.png", 0, 24 );
		const SpriteGraphics mcguffins_cross_ = SpriteGraphics( "charset.png", 8, 40 );

		const Text switch_on_ = { "ON", 192, VERTICAL_POS, Text::FontShade::DARK_GRAY };
		const Text switch_off_ = { "OFF", 192, VERTICAL_POS, Text::FontShade::DARK_GRAY };

		InventoryLevelHealth health_gfx_;
		OxygenMeter oxygen_meter_;
		NewsTicker ticker_;
};