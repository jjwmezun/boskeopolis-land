#pragma once

#include <string>

class ShopItem
{
    public:
        enum class Type
        {
            HP_UPGRADE,
            OXYGEN_UPGRADE
        };

        enum class State
        {
            AVAILABLE,
            NOT_AVAILABLE,
            OUT_OF_STOCK
        };

        ShopItem
        (
            Type type,
            const std::u32string& name,
            const std::u32string& description,
            int price
        );

        const std::u32string& getName() const;
        const std::u32string& getDescription() const;
        int getPrice() const;
        void purchase();
        State getState() const;
        bool isAvailable() const;
        void renderMainIcon( int x, int y ) const;
        void renderSmallIcon( int x, int y ) const;

    private:
        int getMainIconSrc() const;
        int getSmallIconSrc() const;

        Type type_;
        int price_;
        State state_;
        std::u32string name_;
        std::u32string description_;
};