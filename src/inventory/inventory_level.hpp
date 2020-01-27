#pragma once

class Camera;
class EventSystem;
class Health;
class Map;
class Sprite;

#include "inventory_level_health.hpp"
#include "mezun_sdl2.hpp"
#include "oxygen_meter.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"

class InventoryLevel
{
	public:
		static constexpr int HEIGHT = 40;
		static constexpr int Y = Unit::WINDOW_HEIGHT_PIXELS - HEIGHT;
		static constexpr int TOP_ROW_Y_RELATIVE = 10;
		static constexpr int TOP_ROW_Y = Y + TOP_ROW_Y_RELATIVE;
		static constexpr int CLOCK_ICON_X = Unit::MiniBlocksToPixels( 17 );
		static constexpr int CLOCK_X = CLOCK_ICON_X + 8;

		InventoryLevel();
		~InventoryLevel();
		InventoryLevel( const InventoryLevel& ) = delete;
		InventoryLevel( InventoryLevel&& ) = delete;
		InventoryLevel& operator=( const InventoryLevel& ) = delete;
		InventoryLevel& operator=( InventoryLevel&& ) = delete;
		void init();
		void update( EventSystem& events, const Health& health );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera, const Map& lvmap );
		void setShowMcGuffins();
		void setKillCounter( int count );
		void forceRerender();

	private:
		void updateHealthGraphics();
		void updatePtsGraphics();
		void renderPtsGraphics();
		void updateTimerGraphics();
		void renderTimerGraphics();
	/*
		static constexpr int VICTORY_X = Unit::MiniBlocksToPixels( 1 );
		static constexpr int DIAMOND_X = Unit::MiniBlocksToPixels( 2 );
		static constexpr int FUNDS_ICON_X = Unit::MiniBlocksToPixels( 4 );
		static constexpr int FUNDS_X = FUNDS_ICON_X + 8;
		static constexpr int MISC_X = Unit::MiniBlocksToPixels( 23 );

		static constexpr sdl2::SDLRect BG_DEST = { 0, Y, Unit::WINDOW_WIDTH_PIXELS, HEIGHT };
		static constexpr sdl2::SDLRect VICTORY_ICON_DEST = { VICTORY_X, TOP_ROW_Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect DIAMOND_ICON_DEST = { DIAMOND_X, TOP_ROW_Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect GEM_ICON_DEST = { FUNDS_ICON_X, TOP_ROW_Y, 8, 8 };
		static constexpr sdl2::SDLRect CLOCK_ICON_DEST = { CLOCK_ICON_X, TOP_ROW_Y, 8, 8 };

		static constexpr sdl2::SDLRect KEY_ICON_DEST = { MISC_X, TOP_ROW_Y, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
		static constexpr sdl2::SDLRect MCGUFFIN_DEST = { MISC_X, TOP_ROW_Y, 8, 8 };
		static constexpr sdl2::SDLRect MCGUFFIN_CROSS_DEST = { MISC_X + 8, TOP_ROW_Y, 8, 8 };

		static constexpr int HAVE_X = 48;
		static constexpr int DONT_HAVE_X = 56;

		SpriteGraphics victory_gfx_ = SpriteGraphics( "tilesets/universal.png", DONT_HAVE_X, 8 );
		SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", DONT_HAVE_X, 0 );
		const SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 48 );
		const SpriteGraphics clock_gfx_ = SpriteGraphics( "tilesets/universal.png", 96, 48 );

		const SpriteGraphics key_gfx_ = SpriteGraphics( "tilesets/universal.png", 40, 8 );
		const SpriteGraphics mcguffins_gfx_ = SpriteGraphics( "tilesets/universal.png", 0, 24 );
		const SpriteGraphics kills_gfx_ = SpriteGraphics( "tilesets/universal.png", 8, 24 );
		const SpriteGraphics mcguffins_cross_ = SpriteGraphics( "charset.png", 8, 40 );
		const TextObj switch_on_ = { "ON", MISC_X, TOP_ROW_Y, Text::FontColor::DARK_GRAY };
		const TextObj switch_off_ = { "OFF", MISC_X, TOP_ROW_Y, Text::FontColor::DARK_GRAY };
		*/

		InventoryLevelHealth health_gfx_;
		OxygenMeter oxygen_meter_;
		TextObj ticker_;
		int flashing_timer_;
		int flashing_time_shade_;




		TextureBox main_texture_;
		char scratch_[ 3000 ];
};
