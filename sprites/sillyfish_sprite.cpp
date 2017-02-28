




// Name
//===================================
//
// SillyfishSprite
//


// DEPENDENCIES
//===================================

    #include "block_component.h"
    #include "block_system.h"
    #include "collision.h"
    #include "sillyfish_sprite.h"
    #include "sillyfish_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SillyfishSprite::SillyfishSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SillyfishGraphics() ), x, y, 16, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
        start_ground_lock_ ( false ),
        switch_lock_ ( { 4, false } )
    {};

    SillyfishSprite::~SillyfishSprite() {};

    void SillyfishSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
		switch ( directionX() )
		{
			case ( Direction::Horizontal::LEFT ):
				
				if( !blocks.blocksInTheWay
				(
					{
						leftSubPixels() - Unit::BlocksToSubPixels( 1 ),
						bottomSubPixels(),
						Unit::BlocksToSubPixels( 1 ),
						Unit::BlocksToSubPixels( 1 )
						
					},
					BlockComponent::Type::SOLID 
				))
				{
					direction_x_ = Direction::Horizontal::RIGHT;
				}
				
				if ( collidedLeft() )
				{
					direction_x_ = Direction::Horizontal::RIGHT;
				}
				
			break;
				
			case ( Direction::Horizontal::RIGHT ):
				
				if( !blocks.blocksInTheWay
				(
					{
						rightSubPixels(),
						bottomSubPixels(),
						Unit::BlocksToSubPixels( 1 ),
						Unit::BlocksToSubPixels( 1 )
						
					},
					BlockComponent::Type::SOLID 
				))
				{
					direction_x_ = Direction::Horizontal::LEFT;
				}
				
				if ( collidedRight() )
				{
					direction_x_ = Direction::Horizontal::LEFT;
				}
				
			break;
				
			case ( Direction::Horizontal::__NULL ):
				std::cout<<"ERROR: Sillyfish has invalid direction."<<std::endl;
			break;
		}
		
		switch ( directionX() )
		{
			case ( Direction::Horizontal::LEFT ):
				
				moveLeft();
				
			break;
				
			case ( Direction::Horizontal::RIGHT ):
				
				moveRight();
				
			break;
				
			case ( Direction::Horizontal::__NULL ):
				std::cout<<"ERROR: Sillyfish has invalid direction."<<std::endl;
			break;
		}
		
    };

    void SillyfishSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
    };
