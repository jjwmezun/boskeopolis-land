



// Name
//===================================
//
// CloudPlatformSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "cloud_platform_sprite.h"
	#include "map.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    CloudPlatformSprite::CloudPlatformSprite( int x, int y, Direction::Horizontal direction )
    :
        Sprite
		(
			std::unique_ptr<SpriteGraphics>
			(
				new SpriteGraphics
				(
					Graphics::SpriteSheet::LVSPRITES_CLOUD_PLATFORM,
					0,
					0,
					false,
					false,
					0,
					true,
					0,
					-6,
					0,
					10
				)
			),
			x,
			y,
		   64,
			8,
		   {},
		   100,
		   1500,
		   0,
		   0,
		   direction,
		   Direction::Vertical::__NULL,
		   nullptr,
		   SpriteMovement::Type::FLOATING,
		   CameraMovement::PERMANENT,
		   false,
		   false
	  	),
        started_ ( false ),
		map_width_ ( 9999999 )
    {};

    CloudPlatformSprite::~CloudPlatformSprite() {};

    void CloudPlatformSprite::customUpdate
	(
		const Input& input,
		Camera& camera,
		Map& lvmap,
		Game& game,
		EventSystem& events,
		SpriteSystem& sprites,
		BlockSystem& blocks
	)
    {
		map_width_ = lvmap.widthPixels();
		
		switch( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				moveLeft();
			break;
			case ( Direction::Horizontal::RIGHT ):
				moveRight();
			break;
			case ( Direction::Horizontal::__NULL ):
				fullStopX();
			break;
		}
    };

    void CloudPlatformSprite::customInteract
	(
		Collision& my_collision,
		Collision& their_collision,
		Sprite& them,
		BlockSystem& blocks,
		SpriteSystem& sprites,
		Map& lvmap
	)
    {
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( their_collision.collideBottom() )
			{
				them.collideStopYBottom( their_collision.overlapYBottom() );
				them.addToX( vx_ );
				them.addToY( vy_ );
				started_ = true;
			}

			// Ensure cloud platform comes back if it leaves you 'hind on a different platform.
			switch ( direction_x_ )
			{
				case ( Direction::Horizontal::LEFT ):
					if ( xPixels() < them.xPixels() - ( map_width_ / 2 ) )
					{
						hit_box_.x = them.xSubPixels() + Unit::PixelsToSubPixels( ( map_width_ / 2 ) );
					}
				break;

				case ( Direction::Horizontal::RIGHT ):
					if ( xPixels() > them.xPixels() + ( map_width_ / 2 ) )
					{
						hit_box_.x = them.xSubPixels() - Unit::PixelsToSubPixels( ( map_width_ / 2 ) );
					}
				break;
			}
		}
    };
