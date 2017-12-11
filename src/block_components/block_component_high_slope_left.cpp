#include "block.hpp"
#include "block_component_high_slope_left.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentHighSlopeLeft::BlockComponentHighSlopeLeft(){};

BlockComponentHighSlopeLeft::~BlockComponentHighSlopeLeft() {};

void BlockComponentHighSlopeLeft::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( collision.collideAny() )
	{
		sprite.on_slope_ = Direction::Horizontal::LEFT;
		if ( sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels() )
		{
			sprite.collideStopYBottom( 0 );
			if ( sprite.vy_ >= 0 )
			{
				sprite.changeY( std::min( sprite.ySubPixels(), block.bottomSubPixels() - sprite.heightSubPixels() - ( ( sprite.centerXSubPixels() - block.leftSubPixels() ) / 2 ) - 9000 ) );

				if ( sprite.vx_ >= 0 )
					sprite.vx_ /= 1.08;
				else if ( sprite.vx_ <= 0 )
					sprite.vx_ *= 1.08;

				sprite.vx_ -= 3;
			}
		}
	}
};
