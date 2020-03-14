#pragma once

#include <stdexcept>

class InvalidLocalizationLanguageException final : public std::runtime_error
{
    public:
        InvalidLocalizationLanguageException( const std::string& path ) :
            std::runtime_error( "The text info file “" + path + "” has been tampered with. Please redownload game & try ’gain." ) {};
};