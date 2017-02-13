


// Name
//===================================
//
// Block
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_type.h"
    #include "collision.h"
    #include "sprite.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Block::Block
    (
        int x, int y, BlockType* type, int location, int type_id
    )
    :
        Object( x, y, Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
        type_ ( type ),
        location_ ( location ),
        type_id_ ( type_id ),
        destroyed_ ( false )
    {};

    // DO NOT DELETE BLOCK TYPE. Owned by other class; this holds just a copy.
    Block::~Block() {};

    Block::Block( Block&& m )
    :
        Object( Unit::SubPixelsToPixels( m.hit_box_.x ), Unit::SubPixelsToPixels( m.hit_box_.y ), Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
        type_ ( m.type_ ),
        location_ ( m.location_ ),
        type_id_ ( m.type_id_ ),
        destroyed_ ( m.destroyed_ )
    {};

    Block::Block( const Block& c )
    :
        Object( Unit::SubPixelsToPixels( c.hit_box_.x ), Unit::SubPixelsToPixels( c.hit_box_.y ), Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
        type_ ( c.type_ ),
        location_ ( c.location_ ),
        type_id_ ( c.type_id_ ),
        destroyed_ ( c.destroyed_ )
    {};

    Block& Block::operator=( Block&& m )
    {
        hit_box_ = m.hit_box_;
        type_ = m.type_;
        location_ = m.location_;
        type_id_ = m.type_id_;
        destroyed_ = m.destroyed_;
    };

    Block& Block::operator=( const Block& c )
    {
        hit_box_ = c.hit_box_;
        type_ = c.type_;
        location_ = c.location_;
        type_id_ = c.type_id_;
        destroyed_ = c.destroyed_;
    };

    void Block::render( Graphics& graphics, Camera& camera, bool priority )
    {
        if ( type_ != nullptr )
            type_->render( graphics, camera, *this, priority );
    };

    void Block::destroy()
    {
        destroyed_ = true;
    };

    int Block::location() const
    {
        return location_;
    };

    void Block::interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        if ( type_ != nullptr )
        {
            Collision collision = sprite.collide( this );
            type_->interact( collision, sprite, *this, level, events, inventory, camera );
        }
    };

    int Block::typeID() const
    {
        return type_id_;
    };
