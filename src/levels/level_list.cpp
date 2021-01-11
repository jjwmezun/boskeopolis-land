#include "level_data.hpp"
#include "level_list.hpp"
#include "mezun_helpers.hpp"
#include "mezun_json.hpp"

namespace LevelList
{
    std::vector<std::string> code_names_;
    std::string directory_;
    std::vector<LevelData> levels_;
    std::string current_code_name_;

    void loadListFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root );
    void loadLevelDataFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root );

    std::vector<std::string> getListOfCodeNames( const std::string& directory )
    {
        directory_ = directory;
        mezun::loadJSON
        (
            directory + "list.json",
            loadListFunction,
            mezun::charToChar32String( "Failed to load level list. Please redownload game & try ’gain" ),
            mezun::charToChar32String( "Level list has become corrupted. Please redownload game & try ’gain." )
        );
        return code_names_;
    };

    void loadListFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root )
    {
        if ( root.HasMember( "levels" ) & root[ "levels" ].IsArray() )
        {
            for ( const auto& item : root[ "levels" ].GetArray() )
            {
                if ( item.IsString() )
                {
                    current_code_name_ = item.GetString();
                    code_names_.emplace_back( current_code_name_ );
                    mezun::loadJSON
                    (
                        directory_ + current_code_name_ + ".json",
                        loadLevelDataFunction,
                        mezun::charToChar32String( std::string( "Level “" + current_code_name_ + "”’s JSON file cannot be found. Please redownload game." ).c_str() ),
                        mezun::charToChar32String( std::string( "Level “" + current_code_name_ + "”’s JSON file in assets/levels has been corrupted. Please redownload game." ).c_str() )
                    );
                }
            }
        }
    };

    void loadLevelDataFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root )
    {
        LevelData level;
        level.code_name_ = current_code_name_;
        level.cycle_ = 0;
        level.theme_ = 0;
        level.id_ = levels_.size();
        level.time_challenge_ = 0;
        level.gem_challenge_ = 0;
        level.has_card_ = level.has_crown_ = level.has_suits_ = true;
        level.has_secret_goal_ = false;
        levels_.emplace_back( level );
    };
};