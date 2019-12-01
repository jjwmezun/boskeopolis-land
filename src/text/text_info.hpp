#pragma once

#include "char_frame.hpp"
#include <vector>

namespace TextInfo
{
    void init();
    const char* getGameTitle();
    const char32_t* getTitleCreatedBy();
    const char32_t* getInputQuitting();
    const std::vector<CharFrame> getCharacterFrames( char32_t character );
}