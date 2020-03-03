#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "save.hpp"
#include "unit.hpp"

static Save::Status saveDataToBackup( const std::string& name, const SaveData& data );

Save Save::loadFromFile( const std::string& path )
{
    Status status = Status::OK;
    Save save;
    save.removed_ = false;
    save.name_ = mezun::stringReplace( mezun::stringReplace( mezun::charToChar32String( path.c_str() ), mezun::charToChar32String( Main::saveDirectory().c_str() ), U"" ), U".sav", U"" );
    std::ifstream file( path );
    if ( file.is_open() )
    {
        file.read( ( char* )( &save.data_ ), sizeof( SaveData ) );
        if ( !file )
        {
            status = Status::FAILED_TO_READ_MAIN_SAVE;
        }
        else if ( !validateSaveData( save.data_ ) )
        {
            status = Status::INVALID_DATA_IN_LOADED_MAIN_SAVE;
        }
        file.close();
    }
    else
    {
        status = Status::FAILED_TO_LOAD_MAIN_SAVE;
    }

    if ( status != Status::OK )
    {
        save.removed_ = true;
        const std::string backup_filename = Main::backupSavePath( mezun::string32ToString8( save.name_ ) );
        std::ifstream backup_file( backup_filename );
        if ( backup_file.is_open() )
        {
            backup_file.read( ( char* )( &save.data_ ), sizeof( SaveData ) );
            if ( !backup_file )
            {
                save.errors_.push_back( mezun::stringReplace( mezun::charToChar32String( "Failed to read save file “%n” or its backup." ), U"%n", save.name_ ) );
            }
            else if ( !validateSaveData( save.data_ ) )
            {
                save.errors_.push_back( mezun::stringReplace( mezun::charToChar32String( "Save file “%n” & its backup have been corrupted & can’t be fixed." ), U"%n", save.name_ ) );
            }
            else if ( status == Status::INVALID_DATA_IN_LOADED_MAIN_SAVE )
            {
                save.errors_.push_back( mezun::stringReplace( mezun::charToChar32String( "Save file “%n” has been corrupted & has been replaced by backup." ), U"%n", save.name_ ) );
                Status save_status = save.save();
                if ( save_status != Status::OK )
                {
                    save.errors_.push_back( mezun::stringReplace( mezun::charToChar32String( "Failed to save file “%n”." ), U"%n", save.name_ ) );
                }
                else
                {
                    save.removed_ = false;
                }
            }
        }
        else
        {
            save.errors_.push_back( mezun::stringReplace( mezun::charToChar32String( "Failed to open save file “%n” & its backup." ), U"%n", save.name_ ) );
        }

    }

    if ( save.removed_ )
    {
        save.deleteSave();
    }

    return save;
};

Save Save::createNew( std::u32string name )
{
    Save save;
    save.removed_ = false;
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
    Status status = Status::OK;

    const std::string name = mezun::string32ToString8( name_ );
    const std::string filename = Main::savePath( name ); // Must be variable so we can get pointer from it.

    // Back up current save file.
    //
    // Note: this will be guaranteed to fail
    // for new files ( since they don’t have backup files ),
    // so we shouldn’t necessarily throw error if it is missing.
    std::ifstream current_file( filename );
    if ( current_file.is_open() )
    {
        SaveData current_save_data;
        current_file.read( ( char* )( &current_save_data ), sizeof( current_save_data ) );
        if ( !current_file )
        {
            // TODO: this is probably always an error,
            // since it’s saying the file loaded, but didn’t
            // read correctly ( probably the file is too small
            // due to corruption ).
        }
        else if ( !validateSaveData( current_save_data ) )
        {
            // Definitely error: read succeeded,
            // but we have proof that the data is invalid.
        }
        else
        {
            saveDataToBackup( name, current_save_data );
        }
        current_file.close();
    }

    // Save main file.
    std::ofstream file( filename );
    if ( file.is_open() )
    {
        file.write( ( char* )( &data_ ), sizeof( data_ ) );
        if ( !file )
        {
            status = status | Status::FAILED_TO_WRITE_TO_MAIN_SAVE;
        }
        file.close();
    }
    else
    {
        status = status | Status::FAILED_TO_LOAD_MAIN_SAVE;
    }

    // Save backup file.
    const Status backup_status = saveDataToBackup( name, data_ );
    if ( testAnyBackupSaveFailStatus( backup_status ) )
    {
        status = status | Status::FAILED_TO_LOAD_BACKUP_SAVE;
    }
    return status;
};

Save::Status Save::deleteSave()
{
    Status status = Status::OK;
    removed_ = true;
    const std::string save_path = Main::savePath( mezun::string32ToString8( name_ ) );
    const char* filename = save_path.c_str();
    const int remove_success = remove( filename );
    if ( remove_success != 0 )
    {
        status = Status::FAILED_TO_DELETE_MAIN_SAVE;
    }
    const std::string backup_save_path = Main::backupSavePath( mezun::string32ToString8( name_ ) );
    const char* backup_filename = backup_save_path.c_str();
    const int backup_remove_success = remove( backup_filename );
    if ( backup_remove_success != 0 )
    {
        status = status | Status::FAILED_TO_DELETE_BACKUP_SAVE;
    }
    return status;
};

Save Save::copy( const std::u32string& name ) const
{
    Save copy = *this;
    copy.name_ = name;
    return copy;
};

const std::u32string& Save::name() const
{
    return name_;
};

bool Save::isDeleted() const
{
    return removed_;
};

Save::Save() {};

bool Save::validateSaveData( const SaveData& data )
{
    return
        data.total_funds_ >= Unit::TOTAL_FUNDS_MIN &&
        data.total_funds_ <= Unit::TOTAL_FUNDS_MAX &&
        data.current_level_ > -1 &&
        data.current_level_ < Level::NUMBER_OF_LEVELS;
};

static Save::Status saveDataToBackup( const std::string& name, const SaveData& data )
{
    Save::Status status = Save::Status::OK;
    const std::string backup_filename = Main::backupSavePath( name );
    std::ofstream backup_file( backup_filename );
    if ( backup_file.is_open() )
    {
        backup_file.write( ( char* )( &data ), sizeof( SaveData ) );
        if ( !backup_file )
        {
            status = status | Save::Status::FAILED_TO_WRITE_TO_BACKUP_SAVE;
        }
        backup_file.close();
    }
    else
    {
        status = Save::Status::FAILED_TO_LOAD_BACKUP_SAVE;
    }
    return status;
};

bool Save::testAnyDeleteFailStatus( Status status )
{
    return status == ( Status::FAILED_TO_DELETE_MAIN_SAVE
        | Status::FAILED_TO_DELETE_BACKUP_SAVE );
};

bool Save::testAnyBackupSaveFailStatus( Status status )
{
    return status == ( Status::FAILED_TO_LOAD_BACKUP_SAVE | Status::FAILED_TO_WRITE_TO_BACKUP_SAVE );
};

const std::vector<std::u32string>& Save::getErrors() const
{
    return errors_;
};

Save Save::createEmpty()
{
    Save save;
    save.name_ = U"";
    save.removed_ = true;
    return save;
};