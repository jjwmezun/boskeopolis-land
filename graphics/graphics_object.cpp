
// Name
//===================================
//
// GraphicsObject
//


// DEPENDENCIES
//===================================

	#include "graphics_object.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    GraphicsObject::GraphicsObject
    (
		sdl2::SDLRect rect,
		SpriteGraphics gfx
    )
    :
		rect_ ( rect ),
		gfx_ ( gfx )
    {};

    GraphicsObject::~GraphicsObject() {};

    void GraphicsObject::update()
    {
		gfx_.update();
    };

    void GraphicsObject::render( Graphics& graphics, Camera* camera, bool priority ) const
    {
        gfx_.render( graphics, rect_, camera, priority );
    };