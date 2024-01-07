#ifndef CONFIG_H
#define CONFIG_H

namespace BSL
{
    constexpr int BLOCK_SIZE = 16;
    constexpr int WINDOW_WIDTH_BLOCKS = 32;
    constexpr int WINDOW_HEIGHT_BLOCKS = 18;
    constexpr int WINDOW_WIDTH_PIXELS = WINDOW_WIDTH_BLOCKS * BLOCK_SIZE;
    constexpr int WINDOW_HEIGHT_PIXELS = WINDOW_HEIGHT_BLOCKS * BLOCK_SIZE;
    constexpr unsigned int MAX_STATES = 5;
}

#endif // CONFIG_H