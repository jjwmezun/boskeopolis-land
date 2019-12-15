#include <cassert>
#include "config.hpp"
#include <filesystem>
#include "input.hpp"
#include "invalid_localization_language_exception.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include <string>
#include <vector>

namespace Localization
{
    static constexpr const char* GAME_TITLE = "Boskeopolis Land";
    static std::vector<LocalizationLanguage> languages;
    static std::vector<std::u32string> language_names;
    static int current_language = 0;

    void init()
    {
        const std::string config_language = Config::getLanguage();

        // Generate LocalizationLanguage object for each file in "resources/localization" directory.
	    const std::string localization_directory = Main::resourcePath() + "localization";
        for ( const auto& file : std::filesystem::directory_iterator( localization_directory ) )
        {
            try
            {
                languages.push_back( LocalizationLanguage( file ) );
            }
            catch ( const InvalidLocalizationLanguageException& e )
            {
                std::cerr << e.what() << std::endl;
            }       
        }

        // Sort languages list by order property.
        std::sort
        (
            std::begin( languages ),
            std::end( languages ),
            []( const LocalizationLanguage& lhs, const LocalizationLanguage& rhs )
            {
                return lhs.getOrder() < rhs.getOrder();
            }
        );

        // Generate list o’ language names.
        for ( const auto& language : languages )
        {
            language_names.push_back( language.getLanguageName() );
        }
        assert( languages.size() == language_names.size() );

        if ( config_language != "" )
        {
            for ( int i = 0; i < languages.size(); ++i )
            {
                if ( languages[ i ].getPathName() == config_language )
                {
                    current_language = i;
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
        return languages[ current_language ];
    };

    const std::vector<std::u32string>& getLanguageNames()
    {
        return language_names;
    };

    void setLanguage( int language_index )
    {
        current_language = language_index;
        Input::changeQuittingText();
        // TODO: If mo’ need changes like these, implement a mo’ sophisticated observer system.
    };

    int getCurrentLanguageIndex()
    {
        return current_language;
    };
}