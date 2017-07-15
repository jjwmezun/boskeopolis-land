#include "block.h"
#include "block_component_steep_slope_left.h"
#include "collision.h"
#include "sprite.h"

BlockComponentSteepSlopeLeft::BlockComponentSteepSlopeLeft(){};

BlockComponentSteepSlopeLeft::~BlockComponentSteepSlopeLeft() {};

void BlockComponentSteepSlopeLeft::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	if ( collision.collideAny() )
	{
		sprite.on_slope_ = Direction::Horizontal::LEFT;
		if ( sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels() )
		{
			sprite.collideStopYBottom( 0 );
			if ( sprite.vy_ >= 0 )
			{
				sprite.changeY( std::min( sprite.ySubPixels(), block.bottomSubPixels() - sprite.heightSubPixels() - ( ( sprite.centerXSubPixels() - block.leftSubPixels() ) ) - 1000 ) );

				if ( sprite.vx_ >= 0 )
					sprite.vx_ /= 1.16;
				else if ( sprite.vx_ <= 0 )
					sprite.vx_ *= 1.16;

				sprite.vx_ -= 8;
			}
		}
	}
};
