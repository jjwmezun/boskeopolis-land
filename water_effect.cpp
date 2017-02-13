

// Name
//===================================
//
// WaterEffect
//


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "sprite.h"
    #include "water_effect.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    void WaterEffect::testSprite( Sprite* sprite )
    {
        if ( sprite != nullptr )
        {
            if ( sprite->bottomSubPixels() > y_ )
            {
                sprite->swim();
            }
        }
    };

    void WaterEffect::render( Graphics& graphics, Camera& camera )
    {
        if ( Unit::SubPixelsToPixels( y_ ) < camera.bottom() )
        {
            int relative_y = Unit::SubPixelsToPixels( y_ ) - camera.y();

            for ( int x = 0; x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
            {
                graphics.renderObject( Graphics::SpriteSheet::LVTILESET_CITY, { 0, 88, 8, 8 }, { x, relative_y, 8, 8 } );
            }

            for ( int y = relative_y + 8; y < Unit::WINDOW_HEIGHT_PIXELS; y += 8 )
            {
                for ( int x = 0; x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
                {
                    graphics.renderObject( Graphics::SpriteSheet::LVTILESET_CITY, { 8, 88, 8, 8 }, { x, y, 8, 8 } );
                }
            }
        }
    };

    int WaterEffect::yBlocks() const
    {
        return Unit::SubPixelsToBlocks( y_ );
    };
