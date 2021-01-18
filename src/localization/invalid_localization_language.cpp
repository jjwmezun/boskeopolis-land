#include "invalid_localization_language_exception.hpp"
#include "mezun_helpers.hpp"

InvalidLocalizationLanguageException::InvalidLocalizationLanguageException( const std::string& path ) :
    mezun::Exception( mezun::string8ToString32( "The text info file “" + path + "” has been tampered with. Please redownload game & try ’gain." ) )
{};