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

    private:
        Type type_;
        int price_;
        std::u32string name_;
        std::u32string description_;
};