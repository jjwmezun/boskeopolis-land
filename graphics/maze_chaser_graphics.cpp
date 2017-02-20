



// Name
//===================================
//
// MazeChaserGraphics
//


// DEPENDENCIES
//===================================

    #include "maze_chaser_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    MazeChaserGraphics::MazeChaserGraphics( MazeChaserSprite::Type type )
    :
        SpriteGraphics ( typeGraphics( type ), 0, 0, false, false, 0, false, -1, -1, 2, 2 )
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

	const Graphics::SpriteSheet MazeChaserGraphics::typeGraphics( MazeChaserSprite::Type type ) const
	{
		switch( type )
		{
			case ( MazeChaserSprite::Type::SHADOW ):
				return Graphics::SpriteSheet::LVSPRITES_EYEBALL;
			break;
			case ( MazeChaserSprite::Type::AMBUSH ):
				return Graphics::SpriteSheet::LVSPRITES_EYEBALL2;
			break;
			case ( MazeChaserSprite::Type::RANDO ):
				return Graphics::SpriteSheet::LVSPRITES_EYEBALL3;
			break;
			case ( MazeChaserSprite::Type::OBFUSCATING ):
				return Graphics::SpriteSheet::LVSPRITES_EYEBALL4;
			break;
		}
	};
