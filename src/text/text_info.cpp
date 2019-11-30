#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include <stdexcept>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include <string>
#include "text_info.hpp"

class MissingTextInfo : public std::runtime_error
{
    public:
        MissingTextInfo() :
            std::runtime_error( "The text info file “resources/text.json” is missing. Please redownload game & try ’gain." ) {};
};

class InvalidTextInfo : public std::runtime_error
{
    public:
        InvalidTextInfo() :
            std::runtime_error( "The text info file “resources/text.json” has been tampered with. Please redownload game & try ’gain." ) {};
};



namespace TextInfo
{
    static constexpr int GAME_TITLE_LIMIT = 64;
    static constexpr int INPUT_QUITTING_LIMIT = 32;
    static char game_title[ GAME_TITLE_LIMIT ];
    static char32_t input_quitting[ INPUT_QUITTING_LIMIT ] = { '\0' };

    void init()
    {
	    const std::string file_path = Main::resourcePath() + "text.json";
        std::ifstream ifs( file_path );
        if( !ifs.is_open() )
        {
            throw MissingTextInfo();
        }
        rapidjson::IStreamWrapper ifs_wrap( ifs );
        rapidjson::Document document;
        document.ParseStream( ifs_wrap );

        if ( !document.IsObject() )
        {
            throw InvalidTextInfo();
        }

        auto data = document.GetObject();
        if ( data.HasMember( "game_title" ) && data[ "game_title" ].IsString() )
        {
            strncpy( game_title, data[ "game_title" ].GetString(), GAME_TITLE_LIMIT );
        }

        if ( data.HasMember( "input" ) && data[ "input" ].IsObject() )
        {
            const auto input = data[ "input" ].GetObject();
            if ( input.HasMember( "quitting" ) && input[ "quitting" ].IsString() )
            {
                const char* quitting = input[ "quitting" ].GetString();
                mezun::copyCharToChar32( input_quitting, quitting, INPUT_QUITTING_LIMIT );
            }
        }
    };

    const char* getGameTitle()
    {
        return game_title;
    };

    const char32_t* getInputQuitting()
    {
        return input_quitting;
    };
}