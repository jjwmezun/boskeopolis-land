#include "block.hpp"
#include "block_condition_fade_out.hpp"
#include "sprite_graphics.hpp"

BlockConditionFadeOut::BlockConditionFadeOut( int threshold ) : threshold_ ( threshold ) {};
BlockConditionFadeOut::~BlockConditionFadeOut() {};

bool BlockConditionFadeOut::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	if ( block.type() != nullptr && block.type()->graphics() != nullptr )
	{
		return block.type()->graphics()->alpha_ > threshold_;
	}
	else
	{
		return false;
	}
};