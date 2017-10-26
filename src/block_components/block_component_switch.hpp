#pragma once

#include "block_component.hpp"
#include "timers/timer_simple.hpp"

class BlockComponentSwitch : public BlockComponent
{
	public:
		BlockComponentSwitch();
		~BlockComponentSwitch();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
		void update( EventSystem& events, BlockType& type ) override;

	private:
		TimerSimple delay_;
		bool ready_ = true;
};