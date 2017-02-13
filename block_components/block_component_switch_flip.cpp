


// Name
//===================================
//
// BlockComponentSwitchFlip
//


// DEPENDENCIES
//===================================

    #include "block_component_switch_flip.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentSwitchFlip::BlockComponentSwitchFlip() {};

    BlockComponentSwitchFlip::~BlockComponentSwitchFlip() {};

    void BlockComponentSwitchFlip::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        if ( ready_ )
        {
            events.flipSwitch();
            ready_ = false;
        }
    };

    void BlockComponentSwitchFlip::update( EventSystem& events, BlockType& type )
    {
        if ( !ready_ )
        {
            if ( delay_.done() )
            {
                ready_ = true;
                delay_.stop();
            }
            else if ( delay_.on() )
            {
                delay_.update();
            }
            else
            {
                delay_.start();
            }
        }
    };
