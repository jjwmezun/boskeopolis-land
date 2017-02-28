
// Name
//===================================
//
// GraphicsObject
//

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

	#include "bosk_sdl2.h"
    #include "sprite_graphics.h"


// CLASS
//===================================

    class GraphicsObject
    {
        public:
            GraphicsObject
            (
				sdl2::SDLRect rect,
				SpriteGraphics gfx
            );
            ~GraphicsObject();
			
			void update();
            void render( Graphics& graphics, Camera* camera = nullptr, bool priority = false ) const;
			
			
		private:
			SpriteGraphics& gfx_;
			const sdl2::SDLRect rect_;
    };


#endif // GRAPHICS_OBJECT_H

