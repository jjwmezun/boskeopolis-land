#include "block.hpp"
#include "block_condition_fade_cloud.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

BlockConditionFadeCloud::BlockConditionFadeCloud() {};

BlockConditionFadeCloud::~BlockConditionFadeCloud() {};

bool BlockConditionFadeCloud::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
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