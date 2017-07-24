#include "block.hpp"
#include "block_component_high_slope_right.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentHighSlopeRight::BlockComponentHighSlopeRight(){};

BlockComponentHighSlopeRight::~BlockComponentHighSlopeRight() {};

void BlockComponentHighSlopeRight::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( collision.collideAny() )
	{
		sprite.on_slope_ = Direction::Horizontal::RIGHT;
		if ( sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels() )
		{
			sprite.collideStopYBottom( 0 );
			if ( sprite.vy_ >= 0 )
			{
				sprite.changeY( std::min( sprite.ySubPixels(), block.bottomSubPixels() - sprite.heightSubPixels() - ( ( block.rightSubPixels() - sprite.centerXSubPixels() ) / 2 ) - 9000 ) );

				if ( sprite.vx_ <= 0 )
					sprite.vx_ /= 1.08;
				else if ( sprite.vx_ >= 0 )
					sprite.vx_ *= 1.08;

				sprite.vx_ += 3;
			}
		}
	}
};
