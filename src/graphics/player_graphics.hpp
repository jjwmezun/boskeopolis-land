
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

    #include "counter.hpp"
    #include <map>
    #include "sprite_graphics.hpp"
	#include "timer_simple.hpp"


// CLASS
//===================================

    class PlayerGraphics : public SpriteGraphics
    {
        public:
            PlayerGraphics( std::string&& texture = "sprites/autumn.png" );
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
