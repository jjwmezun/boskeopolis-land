#ifndef MAP_H
#define MAP_H

#include "block.hpp"
#include "map_layer.hpp"
#include <memory>
#include "renderer.hpp"
#include <string>
#include <vector>

namespace BSL
{
    class Game;
    class Level;
    class Sprite;

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
            void interact( Sprite & sprite, Level & level, Game & game );

            unsigned int getWidthPixels() const;
            unsigned int getHeightPixels() const;
            bool testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types = { CollisionType::SOLID } ) const;
            constexpr unsigned int getIFromXAndY( unsigned int x, unsigned int y ) const
            {
                return y * width_ + x;
            };
            constexpr void scheduleBlockRemoval()
            {
                remove_block_ = true;
            }

        private:
            std::vector<std::vector<int>> collision_;
            std::vector<std::unique_ptr<MapLayer>> layers_;
            std::vector<std::vector<Block>> blocks_;
            std::vector<TilemapGraphics> block_layers_;
            std::string slug_;
            unsigned int width_;
            unsigned int height_;
            int i_;
            bool remove_block_;
    };
}

#endif // MAP_H