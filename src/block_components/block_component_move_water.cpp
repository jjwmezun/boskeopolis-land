#include "block.hpp"
#include "block_component_move_water.hpp"
#include "collision.hpp"
#include "event_system.hpp"

BlockComponentMoveWater::BlockComponentMoveWater() {};

BlockComponentMoveWater::~BlockComponentMoveWater() {};

void BlockComponentMoveWater::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	if ( events.waterShouldMove() )
	{
		if ( events.move_water_ > events.current_water_ )
		{
			type.rotate( Direction::Clockwise::CLOCKWISE );
		}
		else if ( events.move_water_ < events.current_water_ )
		{
			type.rotate( Direction::Clockwise::COUNTERCLOCKWISE );
		}
	}
	else
	{
		type.readjust();
	}

	if ( collision.collideAny() )
	{
		events.move_water_ = block.bottomSubPixels();
	}
};
