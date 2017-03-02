



// Name
//===================================
//
// MapLayerWater
//

#ifndef MAP_LAYER_WATER_H
#define MAP_LAYER_WATER_H


// FORWARD DECLARATIONS
//===================================

	class Camera;
	class Graphics;
	class Sprite;


// DEPENDENCIES
//===================================

	#include "animated_graphics.h"
	#include "counter.h"
	#include "map_layer.h"
	#include "unit.h"


// CLASS
//===================================

    class MapLayerWater : public MapLayer
    {
        public:
			MapLayerWater( int y_blocks );

			void update( EventSystem& events ) override;
			void render( Graphics& graphics, Camera& camera ) const override;
			void interact( Sprite& sprite ) override;


		private:
			int y_;
			AnimatedGraphics surface_;
			const SpriteGraphics body_;
			Counter x_offset_;
			TimerRepeat move_speed_;
    };


#endif // MAP_LAYER_WATER_H
