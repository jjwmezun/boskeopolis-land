

// Name
//===================================
//
// LevelSelectState
//

#ifndef LEVEL_SELECT_STATE_H
#define LEVEL_SELECT_STATE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "event_system.h"
    #include "game_state.h"
    #include "inventory.h"
    #include "level.h"
    #include "sprite_graphics.h"
    #include "text.h"


// CLASS
//===================================

    class LevelSelectState : public GameState
    {

        public:
            enum class Status
            {
                NEW,
                LOAD,
                CONTINUE
            };

            static LevelSelectState* continueLevelSelect( EventSystem events, InventoryLevel inventory, int level );
            static LevelSelectState* newLevelSelect();
            static LevelSelectState* loadLevelSelect();
            ~LevelSelectState();

            void update( Game& game, const Input& input, Graphics& graphics );
            void stateRender( Graphics& graphics );
            void init( Game& game, Graphics& graphics );
            void backFromPop( Game& game, Graphics& graphics );


        private:
            LevelSelectState( Status status, EventSystem events, Inventory inventory, int level );

            static constexpr int DEFAULT_LEVEL = 0;
            static constexpr int FUNDS_X = 32;
			static constexpr int BG_COLOR = 1;
			static constexpr int INVENTORY_Y = Unit::WINDOW_HEIGHT_PIXELS - 24;
            sdl2::SDLRect INVENTORY_BG_DEST = { 0, INVENTORY_Y, Unit::WINDOW_WIDTH_PIXELS, 24 };
            static constexpr int START_Y = 40;
            sdl2::SDLRect HEADER_BG_DEST = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, START_Y };
            sdl2::SDLRect GEM_ICON_DEST = { FUNDS_X - 16, INVENTORY_Y + 8, 8, 8 };

            Text title_ = Text( "Select a Level, yo.", 0, 16, Text::FontShade::BLACK, Text::FontAlign::CENTER );
            SpriteGraphics win_icon_gfx_ = SpriteGraphics( "tilesets/universal.png", 40, 8 );
            SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 0 );
            SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 16 );

            int delay_length_ = 16;
            int delay_ = 0;

            sdl2::SDLRect highlight_dest_ = { 16, START_Y, Unit::WINDOW_WIDTH_PIXELS - 32, 8 };

            std::vector<Text> level_names_;
			std::vector<int>  level_ids_;
            std::vector<Text> gem_scores_;
            std::vector<Text> time_scores_;

            std::vector<sdl2::SDLRect> win_icon_dests_;
            std::vector<sdl2::SDLRect> diamond_icon_dests_;

            EventSystem events_;
            Inventory inventory_;
            int prev_level_;
            Counter selection_;
            Camera camera_;
            Status status_;
    };

#endif // LEVEL_SELECT_STATE_H
