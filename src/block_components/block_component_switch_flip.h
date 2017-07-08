#ifndef BLOCK_COMPONENT_SWITCH_FLIP_H
#define BLOCK_COMPONENT_SWITCH_FLIP_H

#include "block_component.h"
#include <memory>
#include "timers/timer_simple.h"

class BlockComponentSwitchFlip : public BlockComponent
{
	public:
		BlockComponentSwitchFlip();
		~BlockComponentSwitchFlip();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
		void update( EventSystem& events, BlockType& type ) override;

	private:
		TimerSimple delay_;
		bool ready_ = true;
};

#endif // BLOCK_COMPONENT_SWITCH_FLIP_H

