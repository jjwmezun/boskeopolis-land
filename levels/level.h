


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
    #include "rapidjson/document.h"
    #include <vector>


// CLASS
//===================================

    class Level
    {
        public:
			static constexpr int NUM_O_LEVELS = 64;
			
            static Level getLevel( int id );
			
            ~Level();
            Level( Level&& m );
			Level( const Level& ) = delete;
			Level&& operator=( Level&& ) = delete;
			const Level& operator=( const Level& ) = delete;
			
            Map& currentMap();
            const Map& currentMap() const;

            int entranceX() const;
            int entranceY() const;
            int cameraX() const;
            int cameraY() const;
            void warp( SpriteSystem& sprites, Camera& camera, GameState* state, Graphics& graphics, InventoryLevel& inventory );
            Goal* goal() const;
            std::string message() const;
            int id() const;
			
			void update( EventSystem& events );
			void interact( SpriteSystem& sprites );
			
            static const std::string& NameOLevel( unsigned int n );
			static unsigned int realLevelNum();

        private:
			static std::vector<std::string> level_list_;
			static unsigned int real_level_num_;
			
            const int id_;
            std::vector<Map> maps_;
            std::unique_ptr<Goal> goal_;
            int entrance_x_;
            int entrance_y_;
            const int camera_x_;
            const int camera_y_;
            const std::string message_;
            int current_map_;

            Level
            (
                int id,
                const std::vector<Map>& maps,
                std::unique_ptr<Goal> goal,
                int entrance_x = 0,
                int entrance_y = 0,
                int camera_x = 0,
                int camera_y = 0,
                std::string message = ""
            );

            int testCurrentMap() const;
			
			static void buildLevelList();
    };

#endif // LEVEL_H

