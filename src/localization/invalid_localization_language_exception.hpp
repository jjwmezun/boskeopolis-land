#pragma once

#include "mezun_exceptions.hpp"

class InvalidLocalizationLanguageException final : public mezun::Exception
{
    public:
        InvalidLocalizationLanguageException( const std::string& path );
};