



// Name
//===================================
//
// SewerMonsterGraphics
//


#ifndef SEWER_MONSTER_GRAPHICS
#define SEWER_MONSTER_GRAPHICS


// FORWARD DECLARATIONS
//===================================

    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite_graphics.h"


// CLASS
//===================================

    class SewerMonsterGraphics : public SpriteGraphics
    {
        public:
            SewerMonsterGraphics();
            ~SewerMonsterGraphics();

            void update( Sprite& sprite );
    };


#endif // SEWER_MONSTER_GRAPHICS
