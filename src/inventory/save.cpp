#include <fstream>
#include "inventory.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "save.hpp"

#include <iostream>

Save Save::loadFromFile( const std::string& path )
{
    Save save;
    save.name_ = mezun::stringReplace( mezun::stringReplace( mezun::charToChar32String( path.c_str() ), mezun::charToChar32String( Main::saveDirectory().c_str() ), U"" ), U".sav", U"" );
    return save;
};

Save Save::createNew( std::u32string name )
{
    Save save;
    save.name_ = name;
    save.data_.oxygen_upgrade_ = false;
    save.data_.total_funds_ = 0;
    save.data_.current_level_ = -1;
    save.data_.number_of_health_upgrades_ = 0;
    for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
    {
        save.data_.been_to_level_[ i ] =
            save.data_.victories_[ i ] =
            save.data_.secret_goals_[ i ] =
            save.data_.diamonds_[ i ] =
            save.data_.crowns_[ i ] =
            save.data_.suits_[ i ] = false;
        save.data_.gem_scores_[ i ] = Inventory::DEFAULT_GEM_SCORE;
        save.data_.time_scores_[ i ] = Inventory::DEFAULT_TIME_SCORE;
    }
    return save;
};

bool Save::hasError() const
{
    return !errors_.empty();
};

Save::Status Save::save() const
{
    const std::string filename = Main::savePath( mezun::string32ToString8( name_ ) );
    std::ofstream file( filename );
    if ( file.is_open() )
    {
        file.write( ( char* )( &data_ ), sizeof( data_ ) );
        file.close();
    }
    else
    {
        return Status::FAILED_TO_LOAD_MAIN_SAVE;
    }
    return Status::OK;
};

const std::u32string& Save::name() const
{
    return name_;
};

Save::Save() {};