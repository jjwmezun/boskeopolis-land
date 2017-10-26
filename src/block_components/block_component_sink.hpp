#ifndef BLOCK_COMPONENT_SINK_H
#define BLOCK_COMPONENT_SINK_H

#include "block_component.hpp"

class BlockComponentSink : public BlockComponent
{
	public:
		BlockComponentSink();
		~BlockComponentSink();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_SINK_H

