
// Name
//===================================
//
// PlayerGraphics
//


#ifndef PLAYER_GRAPHICS_H
#define PLAYER_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "counter.h"
    #include <map>
    #include "sprite_graphics.h"
	#include "timer_simple.h"


// CLASS
//===================================

    class PlayerGraphics : public SpriteGraphics
    {
        public:
            PlayerGraphics( Graphics::SpriteSheet texture = Graphics::SpriteSheet::LVSPRITES_AUTUMN );
            ~PlayerGraphics();

            void update( Sprite& sprite );


        private:
            Counter walk_counter_;
            Counter climb_counter_;
            Counter blink_counter_;
			Counter swim_counter_;
			TimerSimple swim_timer_;
    };


#endif // PLAYER_GRAPHICS_H
