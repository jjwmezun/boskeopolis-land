

// Name
//===================================
//
// BlockComponentMoney
//


// DEPENDENCIES
//===================================

    #include "block_component_money.h"
    #include "inventory_level.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentMoney::BlockComponentMoney( int amount ) : amount_ ( amount ) {};

    BlockComponentMoney::~BlockComponentMoney() {};

    void BlockComponentMoney::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        inventory.addFunds( amount_ );
    };
