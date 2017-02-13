
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


// CLASS
//===================================

    class PlayerGraphics : public SpriteGraphics
    {
        public:
            PlayerGraphics( Graphics::SpriteSheet texture = Graphics::SpriteSheet::LVSPRITES_AUTUMN );
            ~PlayerGraphics();

            void update( Sprite& sprite );


        private:
            Counter walk_counter_ = Counter( 0, 3, 0, true );
            Counter climb_counter_ = Counter( 0, 1, 0, true );
            Counter blink_counter_ = Counter( 0, 11, 0, true );
    };


#endif // PLAYER_GRAPHICS_H
