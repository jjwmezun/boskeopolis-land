#include <cassert>
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
    static constexpr char* GAME_TITLE = "Boskeopolis Land";
    static std::vector<LocalizationLanguage> languages;
    static std::vector<std::u32string> language_names;
    static int current_language = 0;

    void init()
    {
	    const std::string localization_directory = Main::resourcePath() + "localization";
        for ( const auto& file : std::filesystem::directory_iterator( localization_directory ) )
        {
            try
            {
                languages.push_back( LocalizationLanguage( file.path() ) );
                language_names.push_back( languages[ languages.size() - 1 ].getLanguageName() );
            }
            catch ( const InvalidLocalizationLanguageException& e )
            {
                std::cerr << e.what() << std::endl;
            }       
        }
        assert( languages.size() == language_names.size() );
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
    };
}