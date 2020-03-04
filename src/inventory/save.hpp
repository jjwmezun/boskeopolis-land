#pragma once

#include "save_data.hpp"
#include <string>
#include <vector>

class Inventory;

class Save
{
    public:
        friend class Inventory;

        enum class Status
        {
            OK                                 = 0,
            FAILED_TO_LOAD_MAIN_SAVE           = 1,
            FAILED_TO_LOAD_BACKUP_SAVE         = 2,
            FAILED_TO_DELETE_MAIN_SAVE         = 4,
            FAILED_TO_DELETE_BACKUP_SAVE       = 8,
            FAILED_TO_WRITE_TO_MAIN_SAVE       = 16,
            FAILED_TO_WRITE_TO_BACKUP_SAVE     = 32,
            INVALID_DATA_IN_LOADED_MAIN_SAVE   = 64,
            INVALID_DATA_IN_LOADED_BACKUP_SAVE = 128,
            FAILED_TO_READ_MAIN_SAVE           = 256,
            FAILED_TO_READ_BACKUP_SAVE         = 512
        };
        static Save loadFromFile( const std::string& path );
        static Save createNew( std::u32string name );
        static Save createEmpty();
        bool hasError() const;
        const std::vector<std::u32string>& getErrors() const;
        Status save() const;
        Status deleteSave();
        Save copy( const std::u32string& name ) const;
        const std::u32string& name() const;
        bool isDeleted() const;
        void setAsDeleted();

        static bool testAnyDeleteFailStatus( Status status );
        static bool testAnyBackupSaveFailStatus( Status status );

    private:
        static bool validateSaveData( const SaveData& data );

        Save();
        bool removed_;
        std::u32string name_;
        std::vector<std::u32string> errors_;
        SaveData data_;
};

constexpr Save::Status operator|( Save::Status a, Save::Status b )
{
    return ( Save::Status )( ( int )( a ) | ( int )( b ) );
};