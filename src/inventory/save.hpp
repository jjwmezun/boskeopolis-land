#pragma once

#include "save_data.hpp"
#include <string>

class Save
{
    public:
        enum class Status
        {
            OK,
            FAILED_TO_LOAD_MAIN_SAVE
        };
        static Save loadFromFile( const std::string& path );
        static Save createNew( std::u32string name );
        bool hasError() const;
        Status save() const;
        const std::u32string& name() const;

    private:
        Save();
        int order_;
        std::u32string name_;
        std::vector<std::u32string> errors_;
        SaveData data_;
};