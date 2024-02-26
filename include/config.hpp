#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

namespace BSL
{
    constexpr int BLOCK_SIZE = 16;
    constexpr int WINDOW_WIDTH_BLOCKS = 32;
    constexpr int WINDOW_HEIGHT_BLOCKS = 18;
    constexpr int WINDOW_WIDTH_PIXELS = WINDOW_WIDTH_BLOCKS * BLOCK_SIZE;
    constexpr int WINDOW_HEIGHT_PIXELS = WINDOW_HEIGHT_BLOCKS * BLOCK_SIZE;
    constexpr unsigned int MAX_STATES = 5;

    constexpr uint_fast32_t pixelsToBlocks( uint_fast32_t p )
    {
        return static_cast<uint_fast32_t> ( p / BLOCK_SIZE );
    };

    constexpr uint_fast32_t blocksToPixels( uint_fast32_t b )
    {
        return b * BLOCK_SIZE;
    };
}

#endif // CONFIG_H