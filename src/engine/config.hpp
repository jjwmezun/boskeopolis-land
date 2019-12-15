#pragma once

#include "rapidjson/document.h"

namespace Config
{
    rapidjson::Document readData();
    void saveData( const rapidjson::Document& document );
    std::string getLanguage();
}