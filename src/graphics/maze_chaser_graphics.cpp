



// Name
//===================================
//
// MazeChaserGraphics
//


// DEPENDENCIES
//===================================

    #include "maze_chaser_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    MazeChaserGraphics::MazeChaserGraphics( MazeChaserSprite::Type type )
    :
        SpriteGraphics ( "sprites/eyeball.png", typesImgXFrame( type ), 0, false, false, 0, false, -1, -1, 2, 2 )
    {};

    MazeChaserGraphics::~MazeChaserGraphics() {};

    void MazeChaserGraphics::update( Sprite& sprite )
    {
		flip_x_ = false;
		flip_y_ = false;
		rotation_ = 0;
	
		switch( sprite.direction_ )
		{
			case ( Direction::Simple::DOWN ):
				rotation_ = -90;
			break;
			
			case ( Direction::Simple::UP ):
				rotation_ = -90;
				flip_x_ = true; // Must be X, 'cause it's rotated.
			break;
			
			case ( Direction::Simple::RIGHT ):
				flip_x_ = true;
			break;
		}
    };

	int MazeChaserGraphics::typesImgXFrame( MazeChaserSprite::Type type ) const
	{
		return ( int )type * 16;
	};