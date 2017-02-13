


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

    #include "clock.h"
	#include "inventory.h"
    #include "sprite_graphics.h"
    #include "bosk_sdl2.h"
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
            sdl2::SDLRect BG_DEST = { 0, Unit::WINDOW_HEIGHT_PIXELS - 32, Unit::WINDOW_WIDTH_PIXELS, 32 };
            sdl2::SDLRect GEM_ICON_DEST = { 8, VERTICAL_POS, 8, 8 };
            sdl2::SDLRect CLOCK_ICON_DEST = { 112, VERTICAL_POS, 8, 8 };
            sdl2::SDLRect KEY_ICON_DEST = { 176, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
            sdl2::SDLRect DIAMOND_ICON_DEST = { 160, VERTICAL_POS, Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK };
            sdl2::SDLRect HEART_ICON_DEST = { 64, VERTICAL_POS, 8, 8 };

            SpriteGraphics gem_icon_gfx_ = SpriteGraphics( Graphics::SpriteSheet::CHARSET, 32, 48 );
            SpriteGraphics heart_icon_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 32, 8 );
            SpriteGraphics clock_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 40, 0 );
            SpriteGraphics key_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 48, 8 );
            SpriteGraphics diamond_gfx_ = SpriteGraphics( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 48, 0 );


            Text marquee_ = Text( "", Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 8, Text::FontShade::DARK_GRAY, Text::FontAlign::LEFT, false, std::unique_ptr<TextComponent> ( new TextComponentMarquee( Direction::Horizontal::LEFT, 1 ) ) );
            std::vector<std::string> marquee_messages_ =
            {
                "Kitties Want Answers In Possible Kitty Kibble Shortage",
                "Thank you for reading this message. This message loves you.",
                "Clean up on aisle Delfino.",
                "Experts declare, SimCity 3000 Marquee + platformer = bad game. 9/10.",
                "NEWSFLASH: Crazy woman running round grabbing floating gems everywhere she goes. Details @ 11.",
                "¡AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!",
                "WARNING: Playing games for too long can be dangerous. Take 15-minute breaks 'tween each session or we'll come to smash your hand with a hammer-& yes, Jacob, we'll find you @ 2456 Acorn Avenue. We know where you live.",
                "NEWSFLASH: Despite all o' my rage, I am still just a rat in a cage. Details @ 11.",
                "Thou shalt not waste thine time reading these dumb newsflashes 'stead o' playing the game-¡Look out for that Bad Apple! ¡You fool!",
                "If you don't know 'bout the tavern, then it is time."
            };
            std::string randomMarqueeMessage() const;


            static constexpr int OXYGEN_METER_WIDTH_MINIBLOCKS = 10;
            static constexpr int OXYGEN_METER_WIDTH_PIXELS = Unit::MiniBlocksToPixels( OXYGEN_METER_WIDTH_MINIBLOCKS );
            sdl2::SDLRect OXYGEN_METER_DEST = { Unit::WINDOW_WIDTH_PIXELS - OXYGEN_METER_WIDTH_PIXELS - 8, VERTICAL_POS, OXYGEN_METER_WIDTH_PIXELS, 8 };
            bool show_oxygen_ = false;
            int oxygen_color_ = 5;
            int hearts_shown_ = 0;


            Text switch_ = { "ON", 192, VERTICAL_POS, Text::FontShade::DARK_GRAY };


			Inventory inventory_;
            Clock clock_;

    };

#endif // INVENTORY_LEVEL_H

