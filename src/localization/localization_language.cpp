#include <fstream>
#include "invalid_localization_language_exception.hpp"
#include "level.hpp"
#include "localization_language.hpp"
#include "mezun_helpers.hpp"
#include "mezun_time.hpp"
#include "rapidjson/istreamwrapper.h"

LocalizationLanguage::LocalizationLanguage( const std::filesystem::directory_entry& file )
:
    order_ ( 0 ),
    charset_height_ ( 0 ),
    default_character_ (),
    title_options_ (),
    options_options_ (),
    language_ (),
    intro_text_ (),
    title_created_by_ (),
    input_quitting_ (),
    screen_option_fullscreen_ (),
    screen_option_window_ (),
    options_title_ (),
    screen_options_title_ (),
    language_options_title_ (),
    controls_options_title_ (),
    level_select_title_ (),
    level_select_cycle_name_ (),
    level_select_percent_symbol_ (),
    path_name_ ( file.path().stem() ),
    charset_image_src_ (),
    charset_ (),
    level_names_ (),
    controls_actions_names_ ()
{
    const std::string path = file.path();

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
    loadOrder( data, path );
    loadIntroMessage( data, path );
    loadLanguageName( data, path );
    loadInputText( data, path );
    loadTitleText( data, path );
    loadOptionsText( data, path );
    loadScreenOptions( data, path );
    loadLevelSelectText( data, path );
    loadLevelText( data, path );
    loadOverworldText( data, path );
    loadNewsTickerText( data, path );
    loadPauseText( data, path );
    loadLevelTileMenuText( data, path );
    loadGoalText( data, path );
    loadSuccessAndFailureText( data, path );
    loadShopText( data, path );
};

int LocalizationLanguage::getOrder() const
{
    return order_;
};

const std::u32string& LocalizationLanguage::getControlsOptionsTitle() const
{
    return controls_options_title_;
};

const std::u32string* LocalizationLanguage::getControlsActionNames() const
{
    return controls_actions_names_;
};

const std::u32string& LocalizationLanguage::getLanguageName() const
{
    return language_;
};

const std::u32string& LocalizationLanguage::getIntroText() const
{
    return intro_text_;
};

const std::u32string& LocalizationLanguage::getTitleCreatedBy() const
{
    return title_created_by_;
};

const std::u32string& LocalizationLanguage::getInputQuitting() const
{
    return input_quitting_;
};

const std::vector<WTextCharacter> LocalizationLanguage::getCharacterFrames( char32_t character ) const
{
    std::unordered_map<char32_t,std::vector<WTextCharacter>>::const_iterator found = charset_.find( character );
    if ( found == charset_.end() )
    {
        return default_character_;
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

const std::string& LocalizationLanguage::getCharsetImageSrc() const
{
    return charset_image_src_;
};

int LocalizationLanguage::getCharsetHeight() const
{
    return charset_height_;
};

void LocalizationLanguage::loadCharset( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "charset" ) || !data[ "charset" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& charset = data[ "charset" ].GetObject();
    if
    (
        !charset.HasMember( "characters" ) ||
        !charset[ "characters" ].IsArray() ||
        !charset.HasMember( "height" )     ||
        !charset[ "height" ].IsInt()       ||
        !charset.HasMember( "image" )      ||
        !charset[ "image" ].IsString()     ||
        !charset.HasMember( "default_character" ) ||
        !charset[ "default_character" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    charset_height_ = charset[ "height" ].GetInt();
    charset_image_src_ = std::string( "charset/" ) + std::string( charset[ "image" ].GetString() );

    const auto& default_character_obj = charset[ "default_character" ].GetObject();
    if
    (
        !default_character_obj.HasMember( "x" ) ||
        !default_character_obj[ "x" ].IsInt()   ||
        !default_character_obj.HasMember( "y" ) ||
        !default_character_obj[ "y" ].IsInt()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    default_character_.push_back( WTextCharacter( default_character_obj[ "x" ].GetInt(), default_character_obj[ "y" ].GetInt(), WTextCharacter::Type::WHITESPACE ) );

    for ( const auto& item : charset[ "characters" ].GetArray() )
    {
        if ( item.IsObject() && item.HasMember( "key" ) && item[ "key" ].IsString() && item.HasMember( "values" ) )
        {
            char32_t key = mezun::copyCharToChar32( item[ "key" ].GetString() );
            std::vector<WTextCharacter> values;
            if
            (
                item[ "values" ].IsObject() &&
                item[ "values" ].HasMember( "x" ) &&
                item[ "values" ][ "x" ].IsInt() &&
                item[ "values" ].HasMember( "y" ) &&
                item[ "values" ][ "y" ].IsInt()
            )
            {
                WTextCharacter::Type type = WTextCharacter::Type::NORMAL;
                if ( item[ "values" ].HasMember( "whitespace" ) && item[ "values" ][ "whitespace" ].IsBool() && item[ "values" ][ "whitespace" ].GetBool() )
                {
                    type = WTextCharacter::Type::WHITESPACE;
                }
                else if ( item[ "values" ].HasMember( "newline" ) && item[ "values" ][ "newline" ].IsBool() && item[ "values" ][ "newline" ].GetBool() )
                {
                    type = WTextCharacter::Type::NEWLINE;
                }
                values.push_back({ item[ "values" ][ "x" ].GetInt(), item[ "values" ][ "y" ].GetInt(), type });
                charset_.insert( std::pair<char32_t, std::vector<WTextCharacter>> ( key, values ) );
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
                        WTextCharacter::Type type = WTextCharacter::Type::NORMAL;
                        if ( subitem.HasMember( "whitespace" ) && subitem[ "whitespace" ].IsBool() && subitem[ "whitespace" ].GetBool() )
                        {
                            type = WTextCharacter::Type::WHITESPACE;
                        }
                        else if ( subitem.HasMember( "newline" ) && subitem[ "newline" ].IsBool() && subitem[ "newline" ].GetBool() )
                        {
                            type = WTextCharacter::Type::NEWLINE;
                        }
                        values.push_back({ subitem[ "x" ].GetInt(), subitem[ "y" ].GetInt(), type });
                    }
                }
                charset_.insert( std::pair<char32_t, std::vector<WTextCharacter>> ( key, values ) );
            }
        }
    }
};

void LocalizationLanguage::loadOrder( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "order" ) || !data[ "order" ].IsInt() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    order_ = data[ "order" ].GetInt();
};

void LocalizationLanguage::loadIntroMessage( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "intro_text" ) || !data[ "intro_text" ].IsString() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    intro_text_ = mezun::charToChar32String( data[ "intro_text" ].GetString() );
};

void LocalizationLanguage::loadLanguageName( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "language" ) || !data[ "language" ].IsString() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    language_ = mezun::charToChar32String( data[ "language" ].GetString() );
};

void LocalizationLanguage::loadInputText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "input" ) || !data[ "input" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto input = data[ "input" ].GetObject();

    if
    (
        !input.HasMember( "quitting" ) ||
        !input[ "quitting" ].IsString() ||
        !input.HasMember( "title" ) ||
        !input[ "title" ].IsString() ||
        !input.HasMember( "change" ) ||
        !input[ "change" ].IsString() ||
        !input.HasMember( "actions" ) ||
        !input[ "actions" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    controls_options_title_ = mezun::charToChar32String( input[ "title" ].GetString() );
    input_quitting_ = mezun::charToChar32String( input[ "quitting" ].GetString() );
    press_any_key_ = mezun::charToChar32String( input[ "change" ].GetString() );

    const auto actions = input[ "actions" ].GetObject();
    for ( int i = 0; i < Input::NUM_O_ACTIONS; ++i )
    {
        if ( !actions.HasMember( Input::TAGS[ i ] ) || !actions[ Input::TAGS[ i ] ].IsString() )
        {
            throw InvalidLocalizationLanguageException( path );
        }
        controls_actions_names_[ i ] = mezun::charToChar32String( actions[ Input::TAGS[ i ] ].GetString() );
    }
};

void LocalizationLanguage::loadScreenOptions( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
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

void LocalizationLanguage::loadTitleText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
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
    std::string title_created_by_str = std::string( input[ "attribution" ].GetString() );
    title_created_by_str = mezun::stringReplace( title_created_by_str, "%Y", mezun::getCurrentYear() );
    title_created_by_ = mezun::charToChar32String( title_created_by_str.c_str() );

    const auto& options = input[ "options" ].GetObject();
    if
    (
        !options.HasMember( "play" ) ||
        !options[ "play" ].IsString() ||
        !options.HasMember( "options" ) ||
        !options[ "options" ].IsString() ||
        !options.HasMember( "quit" ) ||
        !options[ "quit" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    title_options_.emplace_back( mezun::charToChar32String( options[ "play" ].GetString() ) );
    title_options_.emplace_back( mezun::charToChar32String( options[ "options" ].GetString() ) );
    title_options_.emplace_back( mezun::charToChar32String( options[ "quit" ].GetString() ) );
};

void LocalizationLanguage::loadOptionsText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
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

const std::string& LocalizationLanguage::getPathName() const
{
    return path_name_;
};

void LocalizationLanguage::loadLevelSelectText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "level_select" ) || !data[ "level_select" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto level_select = data[ "level_select" ].GetObject();
    if
    (
        !level_select.HasMember( "title" ) ||
        !level_select[ "title" ].IsString() ||
        !level_select.HasMember( "cycle_name" ) ||
        !level_select[ "cycle_name" ].IsString() ||
        !level_select.HasMember( "percent_symbol" ) ||
        !level_select[ "percent_symbol" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    level_select_title_ = mezun::charToChar32String( level_select[ "title" ].GetString() );
    level_select_cycle_name_ = mezun::charToChar32String( level_select[ "cycle_name" ].GetString() );
    level_select_percent_symbol_ = mezun::charToChar32String( level_select[ "percent_symbol" ].GetString() );
};

void LocalizationLanguage::loadLevelText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "levels" ) || !data[ "levels" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& levels = data[ "levels" ].GetObject();
    if
    (
        !levels.HasMember( "names" ) ||
        !levels[ "names" ].IsObject() ||
        !levels.HasMember( "messages" ) ||
        !levels[ "messages" ].IsObject() ||
        !levels.HasMember( "goals" ) ||
        !levels[ "goals" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& level_names = levels[ "names" ].GetObject();
    const auto& level_messages = levels[ "messages" ].GetObject();
    const auto& level_goal_messages = levels[ "goals" ].GetObject();
    for ( int cycle = 1; cycle <= Level::NUMBER_OF_CYCLES; ++cycle )
    {
        for ( int theme_id = 0; theme_id < Level::NUMBER_OF_THEMES; ++theme_id )
        {
            const std::string code_name = std::string( Level::THEMES[ theme_id ] ) + "-" + std::to_string( cycle );
            if ( level_names.HasMember( code_name.c_str() ) && level_names[ code_name.c_str() ].IsString() )
            {
                level_names_.insert
                ({
                    std::string( code_name.c_str() ),
                    mezun::charToChar32String( level_names[ code_name.c_str() ].GetString() )
                });
            }
            if ( level_messages.HasMember( code_name.c_str() ) && level_messages[ code_name.c_str() ].IsString() )
            {
                level_messages_.insert
                ({
                    std::string( code_name.c_str() ),
                    mezun::charToChar32String( level_messages[ code_name.c_str() ].GetString() )
                });
            }
            if ( level_goal_messages.HasMember( code_name.c_str() ) && level_goal_messages[ code_name.c_str() ].IsString() )
            {
                level_goal_messages_.insert
                ({
                    std::string( code_name.c_str() ),
                    mezun::charToChar32String( level_goal_messages[ code_name.c_str() ].GetString() )
                });
            }
        }
    }
};

void LocalizationLanguage::loadOverworldText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "overworld" ) || !data[ "overworld" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& overworld = data[ "overworld" ].GetObject();
    if ( !overworld.HasMember( "menu" ) || !overworld[ "menu" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& menu = overworld[ "menu" ].GetObject();

    if
    (
        !menu.HasMember( "continue" )
        || !menu[ "continue" ].IsString()
        || !menu.HasMember( "level_list" )
        || !menu[ "level_list" ].IsString()
        || !menu.HasMember( "camera_view" )
        || !menu[ "camera_view" ].IsString()
        || !menu.HasMember( "options" )
        || !menu[ "options" ].IsString()
        || !menu.HasMember( "quit" )
        || !menu[ "quit" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    overworld_menu_names_[ 0 ] = mezun::charToChar32String( menu[ "continue" ].GetString() );
    overworld_menu_names_[ 1 ] = mezun::charToChar32String( menu[ "level_list" ].GetString() );
    overworld_menu_names_[ 2 ] = mezun::charToChar32String( menu[ "camera_view" ].GetString() );
    overworld_menu_names_[ 3 ] = mezun::charToChar32String( menu[ "options" ].GetString() );
    overworld_menu_names_[ 4 ] = mezun::charToChar32String( menu[ "quit" ].GetString() );
};

void LocalizationLanguage::loadNewsTickerText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "news_ticker" ) || !data[ "news_ticker" ].IsArray() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    max_news_ticker_message_width_ = 0;
    for ( const auto& message_t : data[ "news_ticker" ].GetArray() )
    {
        if ( message_t.IsString() )
        {
            std::u32string message = mezun::charToChar32String( message_t.GetString() );
            if ( max_news_ticker_message_width_ < message.size() )
            {
                max_news_ticker_message_width_ = message.size();
            }
            news_ticker_messages_.push_back( message );
        }
    }
};

void LocalizationLanguage::loadPauseText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "pause" ) || !data[ "pause" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& pause = data[ "pause" ].GetObject();

    if
    (
        !pause.HasMember( "continue" )
        || !pause[ "continue" ].IsString()
        || !pause.HasMember( "options" )
        || !pause[ "options" ].IsString()
        || !pause.HasMember( "quit_unbeaten" )
        || !pause[ "quit_unbeaten" ].IsString()
        || !pause.HasMember( "quit_beaten" )
        || !pause[ "quit_beaten" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    pause_continue_ = mezun::charToChar32String( pause[ "continue" ].GetString() );
    pause_options_ = mezun::charToChar32String( pause[ "options" ].GetString() );
    pause_quit_unbeaten_ = mezun::charToChar32String( pause[ "quit_unbeaten" ].GetString() );
    pause_quit_beaten_ = mezun::charToChar32String( pause[ "quit_beaten" ].GetString() );
};

void LocalizationLanguage::loadLevelTileMenuText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "level_tile_menu" ) || !data[ "level_tile_menu" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& level_tile_menu = data[ "level_tile_menu" ].GetObject();

    if
    (
        !level_tile_menu.HasMember( "play" )
        || !level_tile_menu[ "play" ].IsString()
        || !level_tile_menu.HasMember( "hard_mode" )
        || !level_tile_menu[ "hard_mode" ].IsString()
        || !level_tile_menu.HasMember( "cancel" )
        || !level_tile_menu[ "cancel" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    level_tile_play_ = mezun::charToChar32String( level_tile_menu[ "play" ].GetString() );
    level_tile_hard_mode_ = mezun::charToChar32String( level_tile_menu[ "hard_mode" ].GetString() );
    level_tile_cancel_ = mezun::charToChar32String( level_tile_menu[ "cancel" ].GetString() );
};

void LocalizationLanguage::loadGoalText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "goals" ) || !data[ "goals" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& goals = data[ "goals" ].GetObject();

    if
    (
        !goals.HasMember( "generic" )
        || !goals[ "generic" ].IsString()
        || !goals.HasMember( "avoid_money" )
        || !goals[ "avoid_money" ].IsString()
        || !goals.HasMember( "collect" )
        || !goals[ "collect" ].IsString()
        || !goals.HasMember( "do_nothing" )
        || !goals[ "do_nothing" ].IsString()
        || !goals.HasMember( "heat" )
        || !goals[ "heat" ].IsString()
        || !goals.HasMember( "mcguffins" )
        || !goals[ "mcguffins" ].IsString()
        || !goals.HasMember( "past_right_edge" )
        || !goals[ "past_right_edge" ].IsString()
        || !goals.HasMember( "starving" )
        || !goals[ "starving" ].IsString()
        || !goals.HasMember( "stop_on_off" )
        || !goals[ "stop_on_off" ].IsString()
        || !goals.HasMember( "survive_time" )
        || !goals[ "survive_time" ].IsString()
        || !goals.HasMember( "timed" )
        || !goals[ "timed" ].IsString()
        || !goals.HasMember( "warp" )
        || !goals[ "warp" ].IsString()
        || !goals.HasMember( "kill_all" )
        || !goals[ "kill_all" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    generic_goal_message_ = mezun::charToChar32String( goals[ "generic" ].GetString() );
    avoid_money_goal_message_ = mezun::charToChar32String( goals[ "avoid_money" ].GetString() );
    collect_goal_message_ = mezun::charToChar32String( goals[ "collect" ].GetString() );
    do_nothing_goal_message_ = mezun::charToChar32String( goals[ "do_nothing" ].GetString() );
    heat_goal_message_ = mezun::charToChar32String( goals[ "heat" ].GetString() );
    mcguffins_goal_message_ = mezun::charToChar32String( goals[ "mcguffins" ].GetString() );
    past_right_edge_goal_message_ = mezun::charToChar32String( goals[ "past_right_edge" ].GetString() );
    starving_goal_message_ = mezun::charToChar32String( goals[ "starving" ].GetString() );
    stop_on_off_goal_message_ = mezun::charToChar32String( goals[ "stop_on_off" ].GetString() );
    survive_time_goal_message_ = mezun::charToChar32String( goals[ "survive_time" ].GetString() );
    timed_goal_message_ = mezun::charToChar32String( goals[ "timed" ].GetString() );
    warp_goal_message_ = mezun::charToChar32String( goals[ "warp" ].GetString() );
    kill_all_goal_message_ = mezun::charToChar32String( goals[ "kill_all" ].GetString() );
};

void LocalizationLanguage::loadSuccessAndFailureText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if
    (
        !data.HasMember( "level_success" )
        || !data[ "level_success" ].IsString()
        || !data.HasMember( "level_failure" )
        || !data[ "level_failure" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    level_success_ = mezun::charToChar32String( data[ "level_success" ].GetString() );
    level_failure_ = mezun::charToChar32String( data[ "level_failure" ].GetString() );
};

void LocalizationLanguage::loadShopText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path )
{
    if ( !data.HasMember( "shop" ) || !data[ "shop" ].IsObject() )
    {
        throw InvalidLocalizationLanguageException( path );
    }
    const auto& shop = data[ "shop" ].GetObject();

    if
    (
        !shop.HasMember( "name" )
        || !shop[ "name" ].IsString()
        || !shop.HasMember( "checkout" )
        || !shop[ "checkout" ].IsString()
        || !shop.HasMember( "exit" )
        || !shop[ "exit" ].IsString()
        || !shop.HasMember( "not_available" )
        || !shop[ "not_available" ].IsString()
        || !shop.HasMember( "out_of_stock" )
        || !shop[ "out_of_stock" ].IsString()
        || !shop.HasMember( "shopkeeper" )
        || !shop[ "shopkeeper" ].IsObject()
        || !shop.HasMember( "items" )
        || !shop[ "items" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& shopkeeper = shop[ "shopkeeper" ].GetObject();
    const auto& items = shop[ "items" ].GetObject();


    if
    (
        !items.HasMember( "hp_upgrade" )
        || !items[ "hp_upgrade" ].IsObject()
        || !items.HasMember( "oxygen_upgrade" )
        || !items[ "oxygen_upgrade" ].IsObject()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    const auto& hp_upgrade = items[ "hp_upgrade" ].GetObject();
    const auto& oxygen_upgrade = items[ "oxygen_upgrade" ].GetObject();

    if
    (
        !shopkeeper.HasMember( "greeting" )
        || !shopkeeper[ "greeting" ].IsString()
        || !shopkeeper.HasMember( "no_items" )
        || !shopkeeper[ "no_items" ].IsString()
        || !shopkeeper.HasMember( "not_enough_money" )
        || !shopkeeper[ "not_enough_money" ].IsString()
        || !shopkeeper.HasMember( "checkout_confirm" )
        || !shopkeeper[ "checkout_confirm" ].IsString()
        || !shopkeeper.HasMember( "thanks" )
        || !shopkeeper[ "thanks" ].IsString()
        || !shopkeeper.HasMember( "leaving" )
        || !shopkeeper[ "leaving" ].IsString()
        || !shopkeeper.HasMember( "not_available" )
        || !shopkeeper[ "not_available" ].IsString()
        || !shopkeeper.HasMember( "out_of_stock" )
        || !shopkeeper[ "out_of_stock" ].IsString()
        || !hp_upgrade.HasMember( "name" )
        || !hp_upgrade[ "name" ].IsString()
        || !hp_upgrade.HasMember( "description" )
        || !hp_upgrade[ "description" ].IsString()
        || !oxygen_upgrade.HasMember( "name" )
        || !oxygen_upgrade[ "name" ].IsString()
        || !oxygen_upgrade.HasMember( "description" )
        || !oxygen_upgrade[ "description" ].IsString()
    )
    {
        throw InvalidLocalizationLanguageException( path );
    }

    shop_item_hp_upgrade_name_ = mezun::charToChar32String( hp_upgrade[ "name" ].GetString() );
    shop_item_hp_upgrade_description_ = mezun::charToChar32String( hp_upgrade[ "description" ].GetString() );
    shop_item_oxygen_upgrade_name_ = mezun::charToChar32String( oxygen_upgrade[ "name" ].GetString() );
    shop_item_oxygen_upgrade_description_ = mezun::charToChar32String( oxygen_upgrade[ "description" ].GetString() );
    shop_no_items_in_cart_message_ = mezun::charToChar32String( shopkeeper[ "no_items" ].GetString() );
    shop_not_enough_funds_for_checkout_ = mezun::charToChar32String( shopkeeper[ "not_enough_money" ].GetString() );
    shop_greeting_ = mezun::charToChar32String( shopkeeper[ "greeting" ].GetString() );
    shop_checkout_confirm_prompt_ = mezun::charToChar32String( shopkeeper[ "checkout_confirm" ].GetString() );
    shop_checkout_thanks_ = mezun::charToChar32String( shopkeeper[ "thanks" ].GetString() );
    shop_not_available_ = mezun::charToChar32String( shopkeeper[ "not_available" ].GetString() );
    shop_out_of_stock_ = mezun::charToChar32String( shopkeeper[ "out_of_stock" ].GetString() );
    shop_leaving_ = mezun::charToChar32String( shopkeeper[ "leaving" ].GetString() );
    shop_checkout_ = mezun::charToChar32String( shop[ "checkout" ].GetString() );
    shop_exit_ = mezun::charToChar32String( shop[ "exit" ].GetString() );
    shopkeeper_not_available_ = mezun::charToChar32String( shop[ "not_available" ].GetString() );
    shopkeeper_out_of_stock_ = mezun::charToChar32String( shop[ "out_of_stock" ].GetString() );
    overworld_shop_title_ = mezun::charToChar32String( shop[ "name" ].GetString() );
};

const std::u32string& LocalizationLanguage::getLevelSelectTitle() const
{
    return level_select_title_;
};

const std::u32string& LocalizationLanguage::getLevelSelectCycleName() const
{
    return level_select_cycle_name_;
};

const std::u32string& LocalizationLanguage::getLevelSelectPercentSymbol() const
{
    return level_select_percent_symbol_;
};

std::u32string LocalizationLanguage::getLevelName( const std::string& code_name ) const
{
    const auto& search = level_names_.find( code_name );
    return ( search == level_names_.end() )
        ? U"MISSING LEVEL"
        : search->second;
};

std::u32string LocalizationLanguage::getLevelMessage( const std::string& code_name ) const
{
    const auto& search = level_messages_.find( code_name );
    return ( search == level_messages_.end() )
        ? U"MISSING LEVEL MESSAGE"
        : search->second;
};

std::u32string LocalizationLanguage::getLevelGoalMessage( const std::string& code_name ) const
{
    const auto& search = level_goal_messages_.find( code_name );
    return ( search == level_goal_messages_.end() )
        ? U""
        : search->second;
};

const std::u32string* LocalizationLanguage::getOverworldMenuNames() const
{
    return overworld_menu_names_;
};

const std::u32string& LocalizationLanguage::getPressAnyKey() const
{
    return press_any_key_;
};

const std::u32string& LocalizationLanguage::getRandomNewsTickerMessage() const
{
    return mezun::randEntry( news_ticker_messages_ );
};

int LocalizationLanguage::getMaxNewsTickerMessageWidth() const
{
    return max_news_ticker_message_width_;
};

const std::u32string& LocalizationLanguage::getPauseContinue() const
{
    return pause_continue_;
};

const std::u32string& LocalizationLanguage::getPauseOptions() const
{
    return pause_options_;
};

const std::u32string& LocalizationLanguage::getPauseQuitUnbeaten() const
{
    return pause_quit_unbeaten_;
};

const std::u32string& LocalizationLanguage::getPauseQuitBeaten() const
{
    return pause_quit_beaten_;
};

const std::u32string& LocalizationLanguage::getLevelTilePlay() const
{
    return level_tile_play_;
};
const std::u32string& LocalizationLanguage::getLevelTileHardMode() const
{
    return level_tile_hard_mode_;
};
const std::u32string& LocalizationLanguage::getLevelTileCancel() const
{
    return level_tile_cancel_;
};

std::u32string LocalizationLanguage::getShopItemHPUpgradeName() const
{
    return shop_item_hp_upgrade_name_;
};

std::u32string LocalizationLanguage::getShopItemHPUpgradeDescription() const
{
    return shop_item_hp_upgrade_description_;
};

std::u32string LocalizationLanguage::getShopItemOxygenUpgradeName() const
{
    return shop_item_oxygen_upgrade_name_;
};

std::u32string LocalizationLanguage::getShopItemOxygenUpgradeDescription() const
{
    return shop_item_oxygen_upgrade_description_;
};

std::u32string LocalizationLanguage::getShopCheckout() const
{
    return shop_checkout_;
};

std::u32string LocalizationLanguage::getShopExit() const
{
    return shop_exit_;
};

std::u32string LocalizationLanguage::getNoItemsInCartMessage() const
{
    return shop_no_items_in_cart_message_;
};

std::u32string LocalizationLanguage::getNotEnoughFundsForCheckoutMessage() const
{
    return shop_not_enough_funds_for_checkout_;
};

std::u32string LocalizationLanguage::getShopGreeting() const
{
    return shop_greeting_;
};

std::u32string LocalizationLanguage::getCheckoutConfirmPrompt() const
{
    return shop_checkout_confirm_prompt_;
};

std::u32string LocalizationLanguage::getCheckoutThanks() const
{
    return shop_checkout_thanks_;
};

std::u32string LocalizationLanguage::getShopNotAvailableText() const
{
    return shop_not_available_;
};

std::u32string LocalizationLanguage::getShopOutOfStockText() const
{
    return shop_out_of_stock_;
}

std::u32string LocalizationLanguage::getShopLeavingMessage() const
{
    return shop_leaving_;
};

std::u32string LocalizationLanguage::getShopkeeperNotAvailableMessage() const
{
    return shopkeeper_not_available_;
};

std::u32string LocalizationLanguage::getShopkeeperOutOfStockMessage() const
{
    return shopkeeper_out_of_stock_;
};

std::u32string LocalizationLanguage::getOverworldShopTitle() const
{
    return overworld_shop_title_;
};

std::u32string LocalizationLanguage::getCollectGoalMessage() const
{
    return collect_goal_message_;
};

std::u32string LocalizationLanguage::getMcGuffinGoalMessage() const
{
    return mcguffins_goal_message_;
};

std::u32string LocalizationLanguage::getSurviveTimeGoalMessage() const
{
    return survive_time_goal_message_;
};

std::u32string LocalizationLanguage::getPastRightEdgeGoalMessage() const
{
    return past_right_edge_goal_message_;
};

std::u32string LocalizationLanguage::getWarpGoalMessage() const
{
    return warp_goal_message_;
};

std::u32string LocalizationLanguage::getAvoidMoneyGoalMessage() const
{
    return avoid_money_goal_message_;
};

std::u32string LocalizationLanguage::getStarvingGoalMessage() const
{
    return starving_goal_message_;
};

std::u32string LocalizationLanguage::getHeatGoalMessage() const
{
    return heat_goal_message_;
};

std::u32string LocalizationLanguage::getDoNothingGoalMessage() const
{
    return do_nothing_goal_message_;
};

std::u32string LocalizationLanguage::getKillAllGoalMessage() const
{
    return kill_all_goal_message_;
};

std::u32string LocalizationLanguage::getStopOnOffGoalMessage() const
{
    return stop_on_off_goal_message_;
};

std::u32string LocalizationLanguage::getTimedMessage() const
{
    return timed_goal_message_;
};

std::u32string LocalizationLanguage::getGenericGoalMessage() const
{
    return generic_goal_message_;
};

std::u32string LocalizationLanguage::getFailureMessage() const
{
    return level_failure_;
};

std::u32string LocalizationLanguage::getSuccessMessage() const
{
    return level_success_;
};

std::u32string LocalizationLanguage::getSecretGoalMessage() const
{
    return mezun::charToChar32String( "¡Secret Goal!" );
};

std::u32string LocalizationLanguage::getMissingPaletteErrorMessage( const std::string& palette_name ) const
{
    return mezun::stringReplace( mezun::charToChar32String( "Palette “%p” is missing. Please redownload game. Hold ESC for a few seconds to exit game." ), U"%p", mezun::charToChar32String( palette_name.c_str() ) );
};