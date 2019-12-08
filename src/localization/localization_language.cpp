#include <fstream>
#include "invalid_localization_language_exception.hpp"
#include "localization_language.hpp"
#include "mezun_helpers.hpp"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

LocalizationLanguage::LocalizationLanguage( const std::string& path )
{
    std::ifstream ifs( path );
    if( !ifs.is_open() )
    {
        throw std::runtime_error( "Unexpected IO error for loading localization: somehow file we found in localization directory failed to be opened." );
    }
    rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document document;
    document.ParseStream( ifs_wrap );

    if ( !document.IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto data = document.GetObject();
    loadCharset( data, path );
    loadLanguageName( data, path );
    loadInputText( data, path );
    loadTitleText( data, path );
    loadOptionsText( data, path );
    loadScreenOptions( data, path );
};

const std::u32string& LocalizationLanguage::getLanguageName() const
{
    return language_;
};

const std::u32string& LocalizationLanguage::getTitleCreatedBy() const
{
    return title_created_by_;
};

const std::u32string& LocalizationLanguage::getInputQuitting() const
{
    return input_quitting_;
};

const std::vector<CharFrame> LocalizationLanguage::getCharacterFrames( char32_t character ) const
{
    std::unordered_map<char32_t,std::vector<CharFrame>>::const_iterator found = charset_.find( character );
    if ( found == charset_.end() )
    {
        return { CharFrame( 30, 3, CharFrame::Type::WHITESPACE ) };
    }
    return found->second;
};

const std::u32string& LocalizationLanguage::getScreenOptionFullscreen() const
{
    return screen_option_fullscreen_;
};

const std::u32string& LocalizationLanguage::getScreenOptionWindow() const
{
    return screen_option_window_;
};

const std::vector<std::u32string>& LocalizationLanguage::getTitleOptions() const
{
    return title_options_;
};

const std::vector<std::u32string>& LocalizationLanguage::getOptionsOptions() const
{
    return options_options_;
};

const std::u32string& LocalizationLanguage::getOptionsTitle() const
{
    return options_title_;
};

const std::u32string& LocalizationLanguage::getScreenOptionsTitle() const
{
    return screen_options_title_;
};

const std::u32string& LocalizationLanguage::getLanguageOptionsTitle() const
{
    return language_options_title_;
};

void LocalizationLanguage::loadCharset( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "charset" ) || !data[ "charset" ].IsArray() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    for ( const auto& item : data[ "charset" ].GetArray() )
    {
        if ( item.IsObject() && item.HasMember( "key" ) && item[ "key" ].IsString() && item.HasMember( "values" ) )
        {
            char32_t key = mezun::copyCharToChar32( item[ "key" ].GetString() );
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
                charset_.insert( std::pair<char32_t, std::vector<CharFrame>> ( key, values ) );
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
                charset_.insert( std::pair<char32_t, std::vector<CharFrame>> ( key, values ) );
            }
        }
    }
};

void LocalizationLanguage::loadLanguageName( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "language" ) || !data[ "language" ].IsString() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    language_ = mezun::charToChar32String( data[ "language" ].GetString() );
};

void LocalizationLanguage::loadInputText( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "input" ) || !data[ "input" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto input = data[ "input" ].GetObject();
    if ( !input.HasMember( "quitting" ) || !input[ "quitting" ].IsString() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    input_quitting_ = mezun::charToChar32String( input[ "quitting" ].GetString() );
};

void LocalizationLanguage::loadScreenOptions( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "screen_options" ) || !data[ "screen_options" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& o = data[ "screen_options" ].GetObject();

    if ( !o.HasMember( "title" ) || !o[ "title" ].IsString() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    screen_options_title_ = mezun::charToChar32String( o[ "title" ].GetString() );

    if ( !o.HasMember( "options" ) || !o[ "options" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& options = o[ "options" ].GetObject();
    if
    (
        !options.HasMember( "fullscreen" ) ||
        !options[ "fullscreen" ].IsString() ||
        !options.HasMember( "window" ) ||
        !options[ "window" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    screen_option_fullscreen_ = mezun::charToChar32String( options[ "fullscreen" ].GetString() );
    screen_option_window_ = mezun::charToChar32String( options[ "window" ].GetString() );
};

void LocalizationLanguage::loadTitleText( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "title" ) || !data[ "title" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto input = data[ "title" ].GetObject();
    if
    (
        !input.HasMember( "attribution" ) ||
        !input[ "attribution" ].IsString() ||
        !input.HasMember( "options" ) ||
        !input[ "options" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    title_created_by_ = mezun::charToChar32String( input[ "attribution" ].GetString() );

    const auto& options = input[ "options" ].GetObject();
    if
    (
        !options.HasMember( "new_game" ) ||
        !options[ "new_game" ].IsString() ||
        !options.HasMember( "load_game" ) ||
        !options[ "load_game" ].IsString() ||
        !options.HasMember( "options" ) ||
        !options[ "options" ].IsString() ||
        !options.HasMember( "quit" ) ||
        !options[ "quit" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    title_options_.emplace_back( mezun::charToChar32String( options[ "new_game" ].GetString() ) );
    title_options_.emplace_back( mezun::charToChar32String( options[ "load_game" ].GetString() ) );
    title_options_.emplace_back( mezun::charToChar32String( options[ "options" ].GetString() ) );
    title_options_.emplace_back( mezun::charToChar32String( options[ "quit" ].GetString() ) );
};

void LocalizationLanguage::loadOptionsText( const auto& data, const std::string& path )
{
    if ( !data.HasMember( "options" ) || !data[ "options" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto input = data[ "options" ].GetObject();
    if
    (
        !input.HasMember( "title" ) ||
        !input[ "title" ].IsString() ||
        !input.HasMember( "options" ) ||
        !input[ "options" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    options_title_ = mezun::charToChar32String( input[ "title" ].GetString() );

    const auto& options = input[ "options" ].GetObject();
    if
    (
        !options.HasMember( "screen_resolution" ) ||
        !options[ "screen_resolution" ].IsString() ||
        !options.HasMember( "controls" ) ||
        !options[ "controls" ].IsString() ||
        !options.HasMember( "language" ) ||
        !options[ "language" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    options_options_.emplace_back( mezun::charToChar32String( options[ "screen_resolution" ].GetString() ) );
    options_options_.emplace_back( mezun::charToChar32String( options[ "controls" ].GetString() ) );
    options_options_.emplace_back( mezun::charToChar32String( options[ "language" ].GetString() ) );
    language_options_title_ = mezun::charToChar32String( options[ "language" ].GetString() );
};