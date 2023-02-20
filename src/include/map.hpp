#ifndef MAP_H
#define MAP_H

#include "map_layer.hpp"
#include <memory>
#include <string>
#include <vector>

namespace BSL
{
    class Game;
    class Level;

    class Map
    {
        public:
            enum class CollisionType
            {
                NONE = 0,
                SOLID = 1,
                SOLID_ABOVE = 2
            };

            Map( std::string && slug );
            void init( Game & game );
            void update( Level & level, const Game & game, float dt );

            unsigned int getWidthPixels() const;
            unsigned int getHeightPixels() const;
            bool testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types = { CollisionType::SOLID } ) const;
            constexpr int getIFromXAndY( unsigned int x, unsigned int y ) const
            {
                return y * width_ + x;
            };

        private:
            std::vector<std::vector<int>> collision_;
            std::vector<std::unique_ptr<MapLayer>> layers_;
            std::string slug_;
            unsigned int width_;
            unsigned int height_;
    };
}

#endif // MAP_H