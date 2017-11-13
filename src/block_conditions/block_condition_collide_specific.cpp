#include "block_condition_collide_specific.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideSpecific::BlockConditionCollideSpecific( int left, int right, int top, int bottom )
:
	left_   ( Unit::PixelsToSubPixels( left   ) ),
	right_  ( Unit::PixelsToSubPixels( right  ) ),
	top_    ( Unit::PixelsToSubPixels( top    ) ),
	bottom_ ( Unit::PixelsToSubPixels( bottom ) )
{};

BlockConditionCollideSpecific::~BlockConditionCollideSpecific() {};

bool BlockConditionCollideSpecific::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events )
{
	return
		( collision.overlapXLeft()   > left_  ||
		collision.overlapXRight()  > right_ ) &&
		( collision.overlapYTop()    > top_   ||
		collision.overlapYBottom() > bottom_ );
};