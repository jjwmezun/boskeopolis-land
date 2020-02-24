#include "save.hpp"

Save Save::loadFromFile( const std::string& path )
{
    Save save;
    return save;
};

bool Save::hasError() const
{
    return !errors_.empty();
};