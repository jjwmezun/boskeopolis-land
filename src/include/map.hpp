#ifndef MAP_H
#define MAP_H

#include "block.hpp"
#include "config.hpp"
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

            struct SpriteData
            {
                unsigned int type;
                bool activated;
            };

            Map( std::string && slug );
            void init( Game & game, Level & level );
            void update( Level & level, const Game & game, float dt );
            void interact( Sprite & sprite, Level & level, Game & game );

            constexpr unsigned int getWidthBlocks() const { return width_; };
            constexpr unsigned int getHeightBlocks() const { return height_; };
            constexpr unsigned int getWidthPixels() const { return blocksToPixels( width_ ); };
            constexpr unsigned int getHeightPixels() const { return blocksToPixels( height_ ); };
            bool testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types = { CollisionType::SOLID } ) const;
            constexpr unsigned int getIFromXAndY( unsigned int x, unsigned int y ) const
            {
                return y * width_ + x;
            };
            constexpr void scheduleBlockRemoval()
            {
                remove_block_ = true;
            }
            constexpr std::vector<std::vector<SpriteData>> & getSprites()
            {
                return sprites_;
            }

        private:
            std::vector<std::vector<int>> collision_;
            std::vector<std::unique_ptr<MapLayer>> layers_;
            std::vector<std::vector<Block>> blocks_;
            std::vector<TilemapGraphics> block_layers_;
            std::vector<std::vector<SpriteData>> sprites_;
            std::string slug_;
            unsigned int width_;
            unsigned int height_;
            int i_;
            bool remove_block_;
    };
}

#endif // MAP_H