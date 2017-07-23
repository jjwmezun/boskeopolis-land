

// Name
//===================================
//
// MazeChaserGraphics
//


#ifndef MAZE_CHASER_GRAPHICS_H
#define MAZE_CHASER_GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.hpp"
    #include "maze_chaser_sprite.hpp"


// CLASS
//===================================

    class MazeChaserGraphics : public SpriteGraphics
    {
        public:
            MazeChaserGraphics( MazeChaserSprite::Type type );
            ~MazeChaserGraphics();

            void update( Sprite& sprite );
			
		private:
			int typesImgXFrame( MazeChaserSprite::Type type ) const;
    };


#endif // MAZE_CHASER_GRAPHICS_H
