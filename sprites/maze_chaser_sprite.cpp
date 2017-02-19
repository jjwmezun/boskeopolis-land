



// Name
//===================================
//
// MazeChaserSprite
//


// DEPENDENCIES
//===================================

	#include "collision.h"
    #include "maze_chaser_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    MazeChaserSprite::MazeChaserSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_EYEBALL ) ), x, y, 16, 16, { SpriteType::ENEMY }, 1500, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true, true, true )
    {};

    MazeChaserSprite::~MazeChaserSprite() {};

    void MazeChaserSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
    };

    void MazeChaserSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( their_collision.collideAny() )
			{
				them.hurt();
			}
			
			if ( them.rightSubPixels() < xSubPixels() )
			{
				moveLeft();
			}
			else if ( them.xSubPixels() > rightSubPixels() )
			{
				moveRight();
			}
			
			if ( them.bottomSubPixels() < ySubPixels() )
			{
				moveUp();
			}
			else if ( them.ySubPixels() > bottomSubPixels() )
			{
				moveDown();
			}
		}
    };
