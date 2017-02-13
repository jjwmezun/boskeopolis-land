



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

	#include "unit.h"


// CLASS
//===================================

    class WaterEffect
    {
        public:
			constexpr WaterEffect( int y_blocks )
			:
				y_ ( Unit::BlocksToSubPixels( y_blocks ) )
			{};

			void testSprite( Sprite* sprite );
			void render( Graphics& graphics, Camera& camera );
            int yBlocks() const;

		private:
			const int y_;
    };


#endif // WATER_EFFECT_H
