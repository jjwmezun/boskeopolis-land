#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "save.hpp"
#include "unit.hpp"

Save Save::loadFromFile( const std::string& path )
{
    Save save;
    save.name_ = mezun::stringReplace( mezun::stringReplace( mezun::charToChar32String( path.c_str() ), mezun::charToChar32String( Main::saveDirectory().c_str() ), U"" ), U".sav", U"" );
    std::ifstream file( path );
    if ( file.is_open() )
    {
        file.read( ( char* )( &save.data_ ), sizeof( data_ ) );
        file.close();
    }
    return save;
};

Save Save::createNew( std::u32string name )
{
    Save save;
    save.name_ = name;
    save.data_.oxygen_upgrade_ = false;
    save.data_.total_funds_ = 0;
    save.data_.current_level_ = 0;
    save.data_.health_upgrades_[ 0 ] = false;
    save.data_.health_upgrades_[ 1 ] = false;
    save.data_.health_upgrades_[ 2 ] = false;
    for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
    {
        save.data_.been_to_level_[ i ] =
            save.data_.victories_[ i ] =
            save.data_.secret_goals_[ i ] =
            save.data_.diamonds_[ i ] =
            save.data_.crowns_[ i ] =
            save.data_.suits_[ i ] = false;
        save.data_.gem_scores_[ i ] = Unit::DEFAULT_GEM_SCORE;
        save.data_.time_scores_[ i ] = Unit::DEFAULT_TIME_SCORE;
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