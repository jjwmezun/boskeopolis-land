


// Name
//===================================
//
// BlockComponentChangePalette
//


// DEPENDENCIES
//===================================

    #include "block_component_change_palette.h"
    #include "event_system.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentChangePalette::BlockComponentChangePalette( const Palette& palette ) : palette_ ( palette ), used_ ( false ) {};

    BlockComponentChangePalette::~BlockComponentChangePalette() {};

    void BlockComponentChangePalette::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        if ( !used_ )
        {
            events.changePalette( palette_ );
            //used_ = true;
        }
    };
