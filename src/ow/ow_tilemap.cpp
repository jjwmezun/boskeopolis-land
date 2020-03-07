#include <cstdio>
#include "ow_tilemap.hpp"
#include <unordered_set>

static constexpr int NUMBER_OF_SOLID_TILES = 40;
static constexpr int SOLID_TILES[ NUMBER_OF_SOLID_TILES ] =
{
	21, 23, 24, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 124, 125, 126, 127, 108, 109, 110, 111, 9, 10, 144, 145, 146, 160, 162, 132, 148, 59
};

static std::unordered_set<int> solid_tiles_ =
{
	21, 23, 24, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 124, 125, 126, 127, 108, 109, 110, 111, 9, 10, 144, 145, 146, 160, 162, 132, 148, 59
};

static bool testIsSolid( int tile )
{
    return solid_tiles_.find( tile - 1 ) != solid_tiles_.end();
};

OWTileMap::OWTileMap()
:
    width_ ( -1 )
{};

void OWTileMap::init( int width, int height )
{
    width_ = width;
    solids_.reserve( width * height );
    for ( int i = 0; i < width * height; ++i )
    {
        solids_.emplace_back( false );
    }
};

void OWTileMap::updateSolids( const std::vector<int>* bg_tiles, const std::vector<int>* fg_tiles )
{
    /*
    const int min_length = std::min
    (
        bg_tiles[ 0 ].size(),
        std::min
        (
            bg_tiles[ 1 ].size(),
            std::min
            (
                fg_tiles[ 0 ].size(),
                std::min
                (
                    fg_tiles[ 1 ].size(),
                    solids_.size()
                )
            )
        )
    );*/

    for ( int i = 0; i < solids_.size(); ++i )
    {
        solids_[ i ] = 
        (
            testIsSolid( bg_tiles[ 0 ][ i ] ) ||
            testIsSolid( bg_tiles[ 1 ][ i ] ) ||
            testIsSolid( fg_tiles[ 0 ][ i ] ) ||
            testIsSolid( fg_tiles[ 1 ][ i ] ) ||
            (
                bg_tiles[ 0 ][ i ] == 0 &&
                bg_tiles[ 1 ][ i ] == 0 &&
                fg_tiles[ 0 ][ i ] == 0 &&
                fg_tiles[ 1 ][ i ] == 0
            )
        );
    }
};

int OWTileMap::width() const
{
    return width_;
};

bool OWTileMap::isSolid( int i ) const
{
    return solids_[ i ];
};