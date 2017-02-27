



// Name
//===================================
//
// BlockComponentBouncy
//


// DEPENDENCIES
//===================================

    #include "block_component_bouncy.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentBouncy::BlockComponentBouncy( int strength ) : strength_ ( strength ) {};

    BlockComponentBouncy::~BlockComponentBouncy() {};

    void BlockComponentBouncy::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        if ( collision.collideBottom() )
        {
            sprite.bounce( strength_ );
        }
        else if ( collision.collideLeft() )
        {
            sprite.bounceLeft( collision.overlapXLeft() );
        }
        else if ( collision.collideRight() )
        {
            sprite.bounceRight( collision.overlapXRight() );
        }
        else if ( collision.collideTop() )
        {
            sprite.bounceDownward( collision.overlapYTop() );
        }
    };
