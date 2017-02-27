


// Name
//===================================
//
// InventoryLevel
//

#ifndef INVENTORY_LEVEL_H
#define INVENTORY_LEVEL_H


// FORWARD DECLARATIONS
//===================================

    class Graphics;


// DEPENDENCIES
//===================================

    #include "bosk_sdl2.h"
    #include "clock.h"
	#include "inventory.h"
	#include "inventory_health.h"
	#include "news_ticker.h"
    #include "sprite_graphics.h"
    #include "text.h"
    #include "text_component_marquee.h"


// CLASS
//===================================

    class InventoryLevel
    {
        public:
            InventoryLevel();
            InventoryLevel( const InventoryLevel& c );
            InventoryLevel( const Inventory c );

            void update( EventSystem& events, Sprite& hero );
            void render( Graphics& graphics, Level::LevelName level, EventSystem& events );
            void init();

            void addFunds( int n = 100 );
            void loseFunds( int n = 100 );
            void setFunds( int n );
            int funds() const;
            int clockTime() const;
            Clock& clock();
            void adjustOxygen( Sprite& hero );

            bool haveDiamond( Level::LevelName level ) const;
            void getDiamond( Level::LevelName level );
            void won( Level::LevelName level );
            void failed();
            void quit( Level::LevelName level );
			const Inventory& inventory() const;


        private:
            static constexpr int VERTICAL_POS = Unit::WINDOW_HEIGHT_PIXELS - 24;
            static constexpr int FUNDS_X = 16;
            static constexpr int CLOCK_X = 120;
            static constexpr sdl2::SDLRect BG_DEST = { 0, Unit::WINDOW_HEIGHT_PIXELS - 32, Unit::WINDOW_WIDTH_PIXELS, 32 };
            static constexpr sdl2::SDLRect GEM_ICON_DEST = { 8, VERTICAL_POS, 8, 8 };
            static constexpr sdl2::SDLRect CLOCK_ICON_DEST = { 112, VERTICAL_POS, 8, 8 };
            static constexpr sdl2::SDLRect KEY_ICON_DEST = { 176, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
            static constexpr sdl2::SDLRect DIAMOND_ICON_DEST = { 160, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };

            const SpriteGraphics gem_icon_gfx_ = SpriteGraphics( Graphics::SpriteSheet::CHARSET, 32, 48 );
            const SpriteGraphics clock_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 40, 0 );
            const SpriteGraphics key_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 48, 8 );
            const SpriteGraphics diamond_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 48, 0 );

            static constexpr int OXYGEN_METER_WIDTH_MINIBLOCKS = 10;
            static constexpr int OXYGEN_METER_WIDTH_PIXELS = Unit::MiniBlocksToPixels( OXYGEN_METER_WIDTH_MINIBLOCKS );
            sdl2::SDLRect OXYGEN_METER_DEST = { Unit::WINDOW_WIDTH_PIXELS - OXYGEN_METER_WIDTH_PIXELS - 8, VERTICAL_POS, OXYGEN_METER_WIDTH_PIXELS, 8 };
            bool show_oxygen_ = false;
            int oxygen_color_ = 5;

            const Text switch_on_ = { "ON", 192, VERTICAL_POS, Text::FontShade::DARK_GRAY };
            const Text switch_off_ = { "OFF", 192, VERTICAL_POS, Text::FontShade::DARK_GRAY };
			
			Inventory inventory_;
			InventoryHealth health_;
            Clock clock_;
			NewsTicker ticker_;

    };

#endif // INVENTORY_LEVEL_H

