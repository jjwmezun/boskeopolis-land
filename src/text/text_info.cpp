#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include <stdexcept>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include <string>
#include "text_info.hpp"
#include "unit.hpp"
#include <unordered_map>

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
    static constexpr int TITLE_CREATED_BY_LIMIT = 512;
    static constexpr int INPUT_QUITTING_LIMIT = 32;

    static char game_title[ GAME_TITLE_LIMIT ];
    static std::u32string title_created_by;
    static std::u32string input_quitting;
    static std::unordered_map<char32_t, std::vector<CharFrame>> charset;

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
        if ( data.HasMember( "charset" ) && data[ "charset" ].IsArray() )
        {
            for ( const auto& item : data[ "charset" ].GetArray() )
            {
                if ( item.IsObject() && item.HasMember( "key" ) && item[ "key" ].IsString() && item.HasMember( "values" ) )
                {
                    char32_t key;
                    mezun::copyCharToChar32( &key, item[ "key" ].GetString(), 1 );
                    std::vector<CharFrame> values;
                    if
                    (
                        item[ "values" ].IsObject() &&
                        item[ "values" ].HasMember( "x" ) &&
                        item[ "values" ][ "x" ].IsInt() &&
                        item[ "values" ].HasMember( "y" ) &&
                        item[ "values" ][ "y" ].IsInt()
                    )
                    {
                        CharFrame::Type type = CharFrame::Type::NORMAL;
                        if ( item[ "values" ].HasMember( "whitespace" ) && item[ "values" ][ "whitespace" ].IsBool() && item[ "values" ][ "whitespace" ].GetBool() )
                        {
                            type = CharFrame::Type::WHITESPACE;
                        }
                        else if ( item[ "values" ].HasMember( "newline" ) && item[ "values" ][ "newline" ].IsBool() && item[ "values" ][ "newline" ].GetBool() )
                        {
                            type = CharFrame::Type::NEWLINE;
                        }
                        values.push_back({ item[ "values" ][ "x" ].GetInt(), item[ "values" ][ "y" ].GetInt(), type });
                        charset.insert( std::pair<char32_t, std::vector<CharFrame>> ( key, values ) );
                    }
                    else if ( item[ "values" ].IsArray() )
                    {
                        const auto& item_list = item[ "values" ].GetArray();
                        for ( const auto& subitem : item_list )
                        {
                            if
                            (
                                subitem.IsObject() &&
                                subitem.HasMember( "x" ) &&
                                subitem[ "x" ].IsInt() &&
                                subitem.HasMember( "y" ) &&
                                subitem[ "y" ].IsInt()
                            )
                            {
                                CharFrame::Type type = CharFrame::Type::NORMAL;
                                if ( subitem.HasMember( "whitespace" ) && subitem[ "whitespace" ].IsBool() && subitem[ "whitespace" ].GetBool() )
                                {
                                    type = CharFrame::Type::WHITESPACE;
                                }
                                else if ( subitem.HasMember( "newline" ) && subitem[ "newline" ].IsBool() && subitem[ "newline" ].GetBool() )
                                {
                                    type = CharFrame::Type::NEWLINE;
                                }
                                values.push_back({ subitem[ "x" ].GetInt(), subitem[ "y" ].GetInt(), type });
                            }
                        }
                        charset.insert( std::pair<char32_t, std::vector<CharFrame>> ( key, values ) );
                    }
                }
            }
        }

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
                input_quitting = mezun::charToChar32String( quitting, INPUT_QUITTING_LIMIT );
            }
        }

        if ( data.HasMember( "title" ) && data[ "title" ].IsObject() )
        {
            const auto input = data[ "title" ].GetObject();
            if ( input.HasMember( "attribution" ) && input[ "attribution" ].IsString() )
            {
                const char* attribution = input[ "attribution" ].GetString();
                title_created_by = mezun::charToChar32String( attribution, TITLE_CREATED_BY_LIMIT );
            }
        }
    };

    const char* getGameTitle()
    {
        return game_title;
    };

    const std::u32string& getTitleCreatedBy()
    {
        return title_created_by;
    };

    const std::u32string& getInputQuitting()
    {
        return input_quitting;
    };

    const std::vector<CharFrame> getCharacterFrames( char32_t character )
    {
        std::unordered_map<char32_t,std::vector<CharFrame>>::const_iterator found = charset.find( character );
        if ( found == charset.end() )
        {
            return { CharFrame( 30, 3, CharFrame::Type::WHITESPACE ) };
        }
        return found->second;
    };
}