#include <cassert>
#include "level_list.hpp"
#include "ow_tile.hpp"

OWTile::OWTile()
:
    type_ ( Type::__NULL ),
    value_ ( 0 )
{};

OWTile OWTile::createLevel( int value )
{
    return { Type::LEVEL, value };
};

OWTile OWTile::createShop( int value )
{
    return { Type::SHOP, value };
};

OWTile OWTile::createNull()
{
    return { Type::__NULL, 0 };
};

int OWTile::getLevelNumber() const
{
    assert( type_ == Type::LEVEL );
    return value_;
};

bool OWTile::isLevelNumber( int value ) const
{
    assert( type_ == Type::LEVEL );
    return value_ == value;
};

bool OWTile::isLevel() const
{
    return type_ == Type::LEVEL;
};

bool OWTile::isShop() const
{
    return type_ == Type::SHOP;
};

bool OWTile::isNull() const
{
    return type_ == Type::__NULL;
};

bool OWTile::isValid() const
{
    return
    (
        type_ == Type::__NULL ||
        ( type_ == Type::LEVEL && value_ >= 0 && value_ < LevelList::getNumberOfLevels() ) ||
        ( type_ == Type::SHOP )
    );
};

bool OWTile::operator!= ( OWTile tile ) const
{
    return type_ != tile.type_ || value_ != tile.value_;
};

OWTile::OWTile( Type type, int value )
:
    type_ ( type ),
    value_ ( value )
{};