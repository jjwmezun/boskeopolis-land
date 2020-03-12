#include "inventory.hpp"
#include "render.hpp"
#include "shop_item.hpp"

static ShopItem::State getStateByType( ShopItem::Type type );

ShopItem::ShopItem
(
    Type type,
    const std::u32string& name,
    const std::u32string& description,
    int price
)
:
    purchased_ ( false ),
    type_ ( type ),
    price_ ( price ),
    state_ ( getStateByType( type ) ),
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

void ShopItem::purchase()
{
    switch ( type_ )
    {
        case ( Type::HP_UPGRADE ):
        {
            Inventory::giveHPUpgrade( 0 );
        }
        break;

        case ( Type::OXYGEN_UPGRADE ):
        {
            Inventory::giveOxygenUpgrade();
        }
        break;

        case ( Type::SPECIAL_LEVEL_1 ):
        {
            Inventory::unlockSpecialLevel( 1 );
        }
        break;
    }
    state_ = getStateByType( type_ );
    purchased_ = true;
};

ShopItem::State ShopItem::getState() const
{
    return state_;
};

bool ShopItem::isAvailable() const
{
    return state_ == State::AVAILABLE;
};

void ShopItem::renderMainIcon( int x, int y ) const
{
    Render::renderObject( "bg/shop-items.png", { getMainIconSrc(), 0, 16, 16  }, { x, y, 16, 16 } );
};

void ShopItem::renderSmallIcon( int x, int y ) const
{
    Render::renderObject( "bg/shop-items.png", { getSmallIconSrc(), 16, 8, 8 }, { x, y, 8, 8 } );
};

int ShopItem::getMainIconSrc() const
{
    switch ( type_ )
    {
        case ( Type::HP_UPGRADE ):
        {
            return 0;
        }
        break;

        case ( Type::OXYGEN_UPGRADE ):
        {
            return 16;
        }
        break;

        case ( Type::SPECIAL_LEVEL_1 ):
        {
            return 32;
        }
        break;
    }
};

int ShopItem::getSmallIconSrc() const
{
    switch ( type_ )
    {
        case ( Type::HP_UPGRADE ):
        {
            return 0;
        }
        break;

        case ( Type::OXYGEN_UPGRADE ):
        {
            return 8;
        }
        break;

        case ( Type::SPECIAL_LEVEL_1 ):
        {
            return 16;
        }
        break;
    }
};

static ShopItem::State getStateByType( ShopItem::Type type )
{
    switch ( type )
    {
        case ( ShopItem::Type::HP_UPGRADE ):
        {
            return ( Inventory::hasHPUpgrade( 0 ) ) ? ShopItem::State::OUT_OF_STOCK : ShopItem::State::AVAILABLE;
        }
        break;
        case ( ShopItem::Type::OXYGEN_UPGRADE ):
        {
            return ( Inventory::haveOxygenUpgrade() ) ? ShopItem::State::OUT_OF_STOCK : ShopItem::State::AVAILABLE;
        }
        break;
        case ( ShopItem::Type::SPECIAL_LEVEL_1 ):
        {
            return ( Inventory::specialLevelUnlocked( 1 ) ) ? ShopItem::State::OUT_OF_STOCK : ShopItem::State::AVAILABLE;
        }
        break;
    }
};

bool ShopItem::hasBeenPurchased() const
{
    return purchased_;
};