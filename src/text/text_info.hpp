#pragma once

#include "char_frame.hpp"
#include <string>
#include <vector>

namespace TextInfo
{
    void init();
    const char* getGameTitle();
    const std::u32string& getTitleCreatedBy();
    const std::u32string& getInputQuitting();
    const std::vector<CharFrame> getCharacterFrames( char32_t character );
}