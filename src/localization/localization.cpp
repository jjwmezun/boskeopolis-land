#include <cassert>
#include "config.hpp"
#include <filesystem>
#include "input.hpp"
#include "invalid_localization_language_exception.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include <string>
#include <vector>
#include "wmessage_state.hpp"

namespace Localization
{
    static constexpr const char* GAME_TITLE = "Boskeopolis Land";
    static std::vector<LocalizationLanguage> languages_ { LocalizationLanguage() };
    static std::vector<std::u32string> language_names_ { U"fallback" };
    static int current_language_ = 0;

    void init()
    {
        const std::string config_language = Config::getLanguage();

        // Generate LocalizationLanguage object for each file in "resources/localization" directory.
	    const std::string localization_directory = Main::resourcePath() + "localization";

        std::vector<LocalizationLanguage> languages {};
        try
        {
            for ( const auto& file : std::filesystem::directory_iterator( localization_directory ) )
            {
                try
                {
                    languages.emplace_back( LocalizationLanguage( file ) );
                }
                catch ( const InvalidLocalizationLanguageException& e )
                {
                    const std::string localization_name = mezun::stringReplace( file.path(), localization_directory, "" );
                    Main::pushState( std::unique_ptr<WMessageState> ( WMessageState::generateErrorMessage( mezun::charToChar32String( mezun::stringReplace( "Localization file “%f” has been corrupted. Please redownload game. Hold ESC for a few seconds to close.", "%f", localization_name ).c_str() ), WMessageState::Type::POP, nullptr ) ) );
                }       
            }
        }
        catch ( const std::filesystem::filesystem_error& exception )
        {
            Main::pushState( std::unique_ptr<WMessageState> ( WMessageState::generateErrorMessage( U"Missing localization folder.\nPlease redownload game.", WMessageState::Type::POP, nullptr ) ) );
        }

        languages_ = languages;        

        // Sort languages list by order property.
        std::sort
        (
            std::begin( languages_ ),
            std::end( languages_ ),
            []( const LocalizationLanguage& lhs, const LocalizationLanguage& rhs )
            {
                return lhs.getOrder() < rhs.getOrder();
            }
        );

        // Generate list o’ language names.
        language_names_.clear();
        for ( const auto& language : languages_ )
        {
            language_names_.emplace_back( language.getLanguageName() );
        }
        assert( languages_.size() == language_names_.size() );

        if ( config_language != "" )
        {
            for ( int i = 0; i < languages_.size(); ++i )
            {
                if ( languages_[ i ].getPathName() == config_language )
                {
                    current_language_ = i;
                }
            }
        }
    };

    const char* getGameTitle()
    {
        return GAME_TITLE;
    };

    const LocalizationLanguage& getCurrentLanguage()
    {
        return languages_[ current_language_ ];
    };

    const std::vector<std::u32string>& getLanguageNames()
    {
        return language_names_;
    };

    void setLanguage( int language_index )
    {
        current_language_ = language_index;
        Input::changeQuittingText();
        // TODO: If mo’ need changes like these, implement a mo’ sophisticated observer system.
    };

    int getCurrentLanguageIndex()
    {
        return current_language_;
    };
}