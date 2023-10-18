#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "dir.hpp"
#include "layer.hpp"
#include <optional>
#include "rect.hpp"
#include <string>
#include <vector>

namespace BSL
{
    class Game;

    enum class WarpType
    {
        NORMAL,
        CLIMB
    };

    struct Warp
    {
        WarpType type = WarpType::NORMAL;
        unsigned int map = 0;
        Rect coords = { 0.0f, 0.0f, 0.0f, 0.0f };
        float entrance_x = 0.0f;
        float entrance_y = 0.0f;
        float camera_x = 0.0f;
        float camera_y = 0.0f;
    };

    struct MapTileLayer
    {
        enum class Type
        {
            __NULL,
            COLLISION,
            TILE,
            OBJECT,
            SPRITE,
            IMAGE,
            WARP,
            GRADIENT,
            RAIN,
            CONSTELLATION,
            PALCHANGE
        };

        struct Image
        {
            char * texture;
            bool tilex;
            bool tiley;
        };

        struct Gradient
        {
            Dir::XY dir;
            unsigned int start;
            unsigned int end;
        };

        struct Rain
        {
            unsigned int start;
            unsigned int end;
        };

        struct PalChange
        {
            unsigned int start;
            unsigned int end;
        };

        union Misc
        {
            Image image;
            Gradient gradient;
            Rain rain;
            PalChange palchange;
        };

        std::vector<int> tiles;
        MapTileLayer::Type type;
        float scrollx;
        float scrolly;
        float offsetx;
        float offsety;
        float tilex;
        Misc misc;
        Layer layer;
    };

    class MapData
    {
        public:
            MapData( const std::string & slug );
            void init( const Game & game );
            void clear();

            const std::string & slug() const;
            const std::vector<MapTileLayer> layers() const;
            unsigned int width() const;
            unsigned int height() const;
            std::optional<Warp> getWarp( const Rect & pos ) const;
            void removeObject( unsigned int layer, unsigned int n );

        private:
            std::string slug_;
            unsigned int width_;
            unsigned int height_;
            bool loaded_;
            std::vector<Warp> warps_;
            std::vector<MapTileLayer> layers_;
            std::vector<unsigned int> object_layers_;
    };
}

#endif // MAP_DATA_H