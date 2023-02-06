#pragma once

#include "assert.hpp"
#include "mezun_sdl2.hpp"

class OWObject final
{
    public:
        enum class Type
        {
            LEVEL,
            SHOP,
            PALETTE,
            WARP
        };
        union Value
        {
            int level;
            int shop;
            int palette;
            int warp;
        };

        constexpr static OWObject createLevel( int level )
        {
            return { Type::LEVEL, Value{ level } };
        };

        constexpr static OWObject createShop( int n )
        {
            return { Type::SHOP, Value{ n } };
        };

        constexpr static OWObject createPaletteChanger( int n )
        {
            return { Type::PALETTE, Value{ n } };
        };

        constexpr static OWObject createWarp( int n )
        {
            return { Type::WARP, Value{ n } };
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

        constexpr int getPaletteID() const
        {
            jassert( type_ == Type::PALETTE );
            return value_.palette;
        };

        constexpr int getWarpValue() const
        {
            jassert( type_ == Type::WARP );
            return value_.warp;
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