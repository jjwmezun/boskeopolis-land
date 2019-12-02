#pragma once

#include "char_frame.hpp"
#include <string>
#include <vector>

namespace TextInfo
{
    void init();
    const char* getGameTitle();
    const std::u32string& getTitleCreatedBy();
    const std::u32string& getScreenOptionFullscreen();
    const std::u32string& getScreenOptionWindow();
    const std::u32string& getInputQuitting();
    const std::u32string& getOptionsTitle();
    const std::u32string& getScreenOptionsTitle();
    const std::vector<CharFrame> getCharacterFrames( char32_t character );
    std::vector<std::u32string>& getTitleOptions();
    std::vector<std::u32string>& getOptionsOptions();
}