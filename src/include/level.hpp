#ifndef LEVEL_H
#define LEVEL_H

#include "level_inventory.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include <string>
#include "tileset.hpp"
#include <unordered_map>

namespace BSL
{
    class Game;

    class Level
    {
        public:
            inline Level()
            :
                slug_ ( "city-1" ),
                current_map_ ( "" ),
                pos_ ( 0.0f, 0.0f, 0.0f, 0.0f )
            {};
            void init( Game & game );
            void update( Game & game, const Controller & controller, float dt );
            inline void setPos( const Rect & pos ) { pos_ = pos; };
            void startWarp( Game & game );
            void doWarp( Game & game, const Controller & controller );

            inline Map & currentMap() { return current_map_; };
            inline LevelInventory & inventory() { return inventory_; };
            inline SpriteSystem & sprites() { return sprites_; };
            inline const Rect & getPos() const { return pos_; };

            Tileset & getTileset( std::string name );

        private:
            std::string slug_;
            Map current_map_;
            SpriteSystem sprites_;
            LevelInventory inventory_;
            Rect pos_;
            std::unordered_map<std::string, Tileset> tilesets_;
            std::vector<std::string> maps_;
    };
}

#endif // LEVEL_H