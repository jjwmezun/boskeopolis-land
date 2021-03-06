#include "ow_tilemap.hpp"
#include <unordered_set>

static std::unordered_set<int> solid_tiles_ =
{
	21, 23, 24, 25, 26, 27, 28, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 124, 125, 126, 127, 108, 109, 110, 111, 9, 10, 144, 145, 146, 160, 162, 132, 148, 59, 78, 61, 62, 63, 77, 79, 93, 94, 95, 185, 187, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 219, 209, 210, 211, 215, 231
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
    const int size = ( int )( solids_.size() );
    for ( int i = 0; i < size; ++i )
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