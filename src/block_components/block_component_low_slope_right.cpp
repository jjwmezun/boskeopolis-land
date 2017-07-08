#include "block.h"
#include "block_component_low_slope_right.h"
#include "collision.h"
#include "sprite.h"

BlockComponentLowSlopeRight::BlockComponentLowSlopeRight(){};

BlockComponentLowSlopeRight::~BlockComponentLowSlopeRight() {};

void BlockComponentLowSlopeRight::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	if ( collision.collideAny() )
	{
		sprite.on_slope_ = Direction::Horizontal::RIGHT;
		if ( sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels() )
		{
			sprite.collideStopYBottom( 0 );
			if ( sprite.vy_ >= 0 )
			{
				sprite.changeY( std::min( sprite.ySubPixels(), block.bottomSubPixels() - sprite.heightSubPixels() - ( ( block.rightSubPixels() - sprite.centerXSubPixels() ) / 2 ) - 1000 ) );

				if ( sprite.vx_ <= 0 && !sprite.isDucking() )
					sprite.vx_ /= 1.08;
				else if ( sprite.vx_ >= 0 )
					sprite.vx_ *= 1.08;
			}
		}
	}
};
