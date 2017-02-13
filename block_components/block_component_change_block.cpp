



// Name
//===================================
//
// BlockComponentChangeBlock
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_component_change_block.h"
    #include "level.h"
    #include "map.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentChangeBlock::BlockComponentChangeBlock( int new_block ) : new_block_ ( new_block ) {};

    BlockComponentChangeBlock::~BlockComponentChangeBlock() {};

    void BlockComponentChangeBlock::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        level.currentMap().changeBlock( block.location(), new_block_ );
    };
