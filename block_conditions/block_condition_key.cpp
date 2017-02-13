


// Name
//===================================
//
// BlockConditionKey
//


// DEPENDENCIES
//===================================

    #include "block_condition_key.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionKey::BlockConditionKey( bool must_have_key ) : must_have_key_ ( must_have_key ) {};

    BlockConditionKey::~BlockConditionKey() {};

    bool BlockConditionKey::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return ( events.key_ == must_have_key_ );
    };
