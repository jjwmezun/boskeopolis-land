#pragma once

#include "assert.hpp"
#include "mezun_sdl2.hpp"

class OWObject
{
    public:
        enum class Type
        {
            LEVEL,
            SHOP
        };
        union Value
        {
            int level;
            int shop;
        };

        constexpr static OWObject createLevel( int level )
        {
            return { Type::LEVEL, Value{ level } };
        };

        constexpr static OWObject createShop( int n )
        {
            return { Type::SHOP, Value{ n } };
        };

        constexpr Type getType() const
        {
            return type_;
        };

        constexpr int getLevelValue() const
        {
            jassert( type_ == Type::LEVEL );
            return value_.level;
        };

        constexpr int getShopNumber() const
        {
            jassert( type_ == Type::SHOP );
            return value_.shop;
        };

    private:
        constexpr OWObject( Type type, Value value )
        :
            type_ ( type ),
            value_ ( value )
        {};

        Type type_;
        Value value_;
};