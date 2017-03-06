
// Name
//===================================
//
// BlockComponentSolid
//


// DEPENDENCIES
//===================================

    #include "block_component_solid.h"
	#include "block_type.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentSolid::BlockComponentSolid()
	:
		BlockComponent( Type::SOLID )
	{};

    BlockComponentSolid::~BlockComponentSolid() {};

    void BlockComponentSolid::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        sprite.collideStopAny( collision );
    };
