


// Name
//===================================
//
// OxygenMeter
//

#ifndef OXYGEN_METER_H
#define OXYGEN_METER_H


// FORWARD DECLARATIONS
//===================================

    class Graphics;


// DEPENDENCIES
//===================================

	#include "sprite_graphics.h"


// CLASS
//===================================

    class OxygenMeter
    {
        public:
			OxygenMeter( int y );
            void update( Sprite& hero );
            void render( Graphics& graphics ) const;


        private:
            static constexpr int WIDTH_MINIBLOCKS = 10;
            static constexpr int HEIGHT_MINIBLOCKS = 1;
            static constexpr int WIDTH_PIXELS = Unit::MiniBlocksToPixels( WIDTH_MINIBLOCKS );
			static constexpr int HEIGHT_PIXELS = Unit::MiniBlocksToPixels( HEIGHT_MINIBLOCKS );
			static constexpr int X_PIXELS = Unit::WINDOW_WIDTH_PIXELS - WIDTH_PIXELS - Unit::MiniBlocksToPixels( 1 );
			
			static constexpr int FIRST_BLOCK = 0;
			static constexpr int LAST_BLOCK = WIDTH_MINIBLOCKS - 1;
			
            const SpriteGraphics gfx_left_bar_;
            const SpriteGraphics gfx_middle_bar_;
            const SpriteGraphics gfx_right_bar_;
			
            sdl2::SDLRect meter_bar_;
			
            bool show_;
            int color_;
			
			void color( double percent );
            void renderMeter( Graphics& graphics ) const;
            void renderShell( Graphics& graphics ) const;
    };

#endif // OXYGEN_METER_H

