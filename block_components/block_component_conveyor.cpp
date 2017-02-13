



// Name
//===================================
//
// BlockComponentConveyor
//


// DEPENDENCIES
//===================================

    #include "block_component_conveyor.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentConveyor::BlockComponentConveyor( Direction::Horizontal direction, int speed )
    :
        x_effect_ ( ( direction == Direction::Horizontal::LEFT ) ? -speed : speed )
    {};

    BlockComponentConveyor::~BlockComponentConveyor() {};

    void BlockComponentConveyor::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        sprite.addToX( x_effect_ );
    };
