#include "shop_item.hpp"

ShopItem::ShopItem
(
    Type type,
    const std::u32string& name,
    const std::u32string& description,
    int price
)
:
    type_ ( type ),
    price_ ( price ),
    name_ ( name ),
    description_ ( description )
{};

const std::u32string& ShopItem::getName() const
{
    return name_;
};

const std::u32string& ShopItem::getDescription() const
{
    return description_;
};

int ShopItem::getPrice() const
{
    return price_;
};