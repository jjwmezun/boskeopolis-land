#include "block.hpp"
#include "block_component_steep_slope_left.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSteepSlopeLeft::BlockComponentSteepSlopeLeft(){};

BlockComponentSteepSlopeLeft::~BlockComponentSteepSlopeLeft() {};

void BlockComponentSteepSlopeLeft::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
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
