


// Name
//===================================
//
// InventoryHealth
//


// DEPENDENCIES
//===================================

    #include "graphics.h"
    #include "inventory_health.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

	InventoryHealth::InventoryHealth( int y )
	:
		dest_ ( { 64, y, 8, 8 } ),
		gfx_ ( Graphics::SpriteSheet::LVTILESET_UNIVERSAL, 32, 8 ),
		hearts_shown_ ( 0 )
	{};

    void InventoryHealth::update( Sprite& hero )
    {
        if ( hearts_shown_ > hero.status().hp() )
        {
            --hearts_shown_;
        }

        if ( hearts_shown_ < hero.status().hp() )
        {
            ++hearts_shown_;
        }

        if ( hearts_shown_ < 0 )
        {
            hearts_shown_ = 0;
        }
    };

    void InventoryHealth::render( Graphics& graphics )
    {
        const int heart_x_mem = dest_.x;
		
        for ( int i = 0; i < hearts_shown_; ++i )
        {
            gfx_.render( graphics, dest_, nullptr );
            dest_.x += 8;
        }
		
        dest_.x = heart_x_mem;
    };