


// Name
//===================================
//
// InventoryHealth
//

#ifndef INVENTORY_HEALTH_H
#define INVENTORY_HEALTH_H


// FORWARD DECLARATIONS
//===================================

	class Graphics;
	class Sprite;


// DEPENDENCIES
//===================================

	#include "bosk_sdl2.h"
	#include "sprite_graphics.h"


// CLASS
//===================================

    class InventoryHealth
    {
        public:
            InventoryHealth( int y );
			void update( Sprite& hero );
			void render( Graphics& graphics );


        private:
            sdl2::SDLRect dest_;
            const SpriteGraphics gfx_;
			int hearts_shown_;

    };

#endif // INVENTORY_HEALTH_H

