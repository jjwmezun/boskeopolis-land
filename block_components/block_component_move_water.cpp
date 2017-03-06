



// Name
//===================================
//
// BlockComponentMoveWater
//


// DEPENDENCIES
//===================================

	#include "block.h"
    #include "block_component_move_water.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentMoveWater::BlockComponentMoveWater() {};

    BlockComponentMoveWater::~BlockComponentMoveWater() {};

    void BlockComponentMoveWater::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
		events.move_water_ = block.bottomSubPixels();
    };
