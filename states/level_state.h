

// Name
//===================================
//
// LevelState
//

#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "block_system.h"
    #include "camera.h"
    #include "game_state.h"
    #include "event_system.h"
    #include "inventory_level.h"
    #include "level.h"
    #include "sprite.h"
    #include "player_graphics.h"
    #include "sprite_system.h"


// CLASS
//===================================

    class LevelState : public GameState
    {

        public:
            LevelState( EventSystem events, InventoryLevel inventory, Level::LevelName lvname );
            ~LevelState();

            void update( Game& game, Input& input, Graphics& graphics );
            void stateRender( Graphics& graphics );
            void init( Game& game, Graphics& graphics );
            void backFromPop( Game& game, Graphics& graphics );

        private:
            Level level_;
            Camera camera_;
            EventSystem events_;
            InventoryLevel inventory_;
            BlockSystem blocks_;
            SpriteSystem sprites_;
    };

#endif // LEVEL_STATE_H
