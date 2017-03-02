

// Name
//===================================
//
// MapLayerWater
//


// DEPENDENCIES
//===================================

    #include "camera.h"
	#include "event_system.h"
    #include "sprite.h"
    #include "map_layer_water.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

	MapLayerWater::MapLayerWater( int y_blocks )
	:
		y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
		surface_
		(
			Graphics::SpriteSheet::LVTILESET_CITY,
			{
				std::make_pair<int, int> ( 144, 120 ),
				std::make_pair<int, int> ( 152, 120 ),
				std::make_pair<int, int> ( 160, 120 ),
				std::make_pair<int, int> ( 168, 120 )
			}
		),
		body_
		(
			Graphics::SpriteSheet::LVTILESET_CITY,
			8,
			88
		),
		x_offset_ ( -8, 0, -8, true ),
		move_speed_ ( 16 )
	{};

    void MapLayerWater::interact( Sprite& sprite )
    {
		if ( sprite.bottomSubPixels() > y_ )
		{
			sprite.swim();
		}
    };

    void MapLayerWater::render( Graphics& graphics, Camera& camera ) const
    {
        if ( Unit::SubPixelsToPixels( y_ ) < camera.bottom() )
        {
            const int relative_y = Unit::SubPixelsToPixels( y_ ) - camera.y();

            for ( int x = x_offset_(); x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
            {
				const sdl2::SDLRect r = { x, relative_y, 8, 8 };
				surface_.render( graphics, r );
            }

            for ( int y = relative_y + 8; y < Unit::WINDOW_HEIGHT_PIXELS; y += 8 )
            {
                for ( int x = x_offset_(); x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
                {
					const sdl2::SDLRect r = { x, y, 8, 8 };
					body_.render( graphics, r );
                }
            }
        }
    };

	void MapLayerWater::update( EventSystem& events )
	{
		surface_.update();
		
		if ( move_speed_.hit() )
		{
			++x_offset_;
		}

		move_speed_.update();
		
		if ( events.waterShouldMove() )
		{
			if ( events.move_water_ < y_ )
			{
				--y_;
			}
			else if ( events.move_water_ > y_ )
			{
				++y_;
			}
		}
	};