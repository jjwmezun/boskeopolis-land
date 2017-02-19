


// Name
//===================================
//
// Level
//

#ifndef LEVEL_H
#define LEVEL_H


// FORWARD DECLARATIONS
//===================================

    class Camera;
    class GameState;
    class Graphics;
    class InventoryLevel;
    class Sprite;
    class SpriteSystem;


// DEPENDENCIES
//===================================

    #include "goal.h"
    #include "map.h"
    #include <vector>


// CLASS
//===================================

    class Level
    {
        public:
            enum LevelName
            {
                // CYCLE 1
                LV_CITY_1,
                LV_WOODS_1,
                LV_MINES_1,
                LV_MOUNTAIN_1,
                LV_SNOW_1,
                LV_FACTORY_1,

                // CYCLE 2
                LV_CITY_4,
                LV_WOODS_3,
                LV_MINES_2,
                LV_SEWER_1,
                LV_DESERT_2,
                LV_SNOW_2,
                LV_CART,
                LV_SKY_2,

                // CYCLE 3
                LV_PYRAMID_1,
                LV_MAZE_1,
                LV_SKY_1,

                // CYCLE 4
                LV_CITY_2,
                LV_WOODS_2,
                LV_MINES_4,
                LV_DESERT_1,
                LV_SAW,
                LV_SNEAK
            };
            static const int NUM_O_LEVELS = LV_SNEAK + 1;

            static Level makeLevel( LevelName level = LevelName::LV_CITY_1 );
            Level( Level&& m );
            ~Level();
            Map& currentMap();

            int entranceX() const;
            int entranceY() const;
            int cameraX() const;
            int cameraY() const;
            void warp( SpriteSystem& sprites, Camera& camera, GameState* state, Graphics& graphics, InventoryLevel& inventory );
            Goal* goal() const;
            static std::string NameOLevel( int n );
            std::string message() const;
            LevelName id() const;

        private:
            const Level::LevelName id_;
            std::vector<Map> maps_;
            std::unique_ptr<Goal> goal_;
            const int entrance_x_;
            const int entrance_y_;
            const int camera_x_;
            const int camera_y_;
            const std::string message_;
            int current_map_;

            Level
            (
                LevelName id,
                const std::vector<Map>& maps,
                std::unique_ptr<Goal> goal,
                int entrance_x = 0,
                int entrance_y = 0,
                int camera_x = 0,
                int camera_y = 0,
                std::string message = ""
            );

            int testCurrentMap();
    };

#endif // LEVEL_H

