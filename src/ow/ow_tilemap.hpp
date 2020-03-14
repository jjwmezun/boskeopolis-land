#pragma once

#include <vector>

class OWTileMap final
{
    public:
        OWTileMap();
        void init( int width, int height );
        void updateSolids( const std::vector<int>* bg_tiles, const std::vector<int>* fg_tiles );
        int width() const;
        bool isSolid( int i ) const;

    private:
        static constexpr int NUMBER_OF_LAYERS = 4;

        int width_;
        std::vector<bool> solids_;
};