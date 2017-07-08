#include "block.h"
#include "block_condition_fade_cloud.h"
#include "sprite.h"
#include "sprite_graphics.h"

BlockConditionFadeCloud::BlockConditionFadeCloud() {};

BlockConditionFadeCloud::~BlockConditionFadeCloud() {};

bool BlockConditionFadeCloud::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	if ( block.type() != nullptr && block.type()->graphics() != nullptr )
	{
		return ( block.type()->graphics()->current_frame_x_ != 4*16 || block.type()->graphics()->current_frame_y_ != 16 ) &&
			( block.type()->graphics()->current_frame_y_ != 16*3 );
	}
	else
	{
		return false;
	}
};