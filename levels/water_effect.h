



// Name
//===================================
//
// WaterEffect
//

#ifndef WATER_EFFECT_H
#define WATER_EFFECT_H


// FORWARD DECLARATIONS
//===================================

	class Camera;
	class Graphics;
	class Sprite;


// DEPENDENCIES
//===================================

	#include "animated_graphics.h"
	#include "counter.h"
	#include "unit.h"


// CLASS
//===================================

    class WaterEffect
    {
        public:
			/*
			constexpr WaterEffect( int y_blocks )
			:
				y_ ( Unit::BlocksToSubPixels( y_blocks ) )
			{};*/
			
			WaterEffect( int y_blocks );

			void update();
			void render( Graphics& graphics, Camera& camera );
            int yBlocks() const;
			void testSprite( Sprite* sprite ) const;


		private:
			const int y_;
			AnimatedGraphics surface_;
			SpriteGraphics body_;
			Counter x_offset_;
			TimerRepeat move_speed_;
    };


#endif // WATER_EFFECT_H
