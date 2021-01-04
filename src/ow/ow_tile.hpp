#pragma once

class OWTile
{
    public:
        OWTile();
        static OWTile createLevel( int value );
        static OWTile createShop( int value );
        static OWTile createNull();

        int getLevelNumber() const;
        bool isLevelNumber( int value ) const;
        bool isLevel() const;
        bool isShop() const;
        bool isNull() const;
        bool isValid() const;

        bool operator!= ( OWTile tile ) const;

    private:
        enum class Type
        {
            __NULL,
            LEVEL,
            SHOP
        };

        OWTile( Type type, int value );

        Type type_;
        int value_;
};