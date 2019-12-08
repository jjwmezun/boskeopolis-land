#pragma once

#include <string>
#include <vector>

class LocalizationLanguage;

namespace Localization
{
    void init();
    const char* getGameTitle();
    const LocalizationLanguage& getCurrentLanguage();
    const std::vector<std::u32string>& getLanguageNames();
    void setLanguage( int language_index );
}