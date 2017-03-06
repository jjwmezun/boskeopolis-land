





// Name
//===================================
//
// SewerMonsterGraphics
//


// DEPENDENCIES
//===================================

    #include "sewer_monster_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SewerMonsterGraphics::SewerMonsterGraphics()
    :
        SpriteGraphics( "sprites/sewer_monster.png", 0, 0, false, false, 0, false, -16, -16, 32, 32 )
    {};

    SewerMonsterGraphics::~SewerMonsterGraphics() {};

    void SewerMonsterGraphics::update( Sprite& sprite )
    {
    };

