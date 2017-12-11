#ifndef BLOCK_COMPONENT_SINGLE_USE_H
#define BLOCK_COMPONENT_SINGLE_USE_H

#include "block_component.hpp"

class BlockComponentSingleUse : public BlockComponent
{
	public:
		BlockComponentSingleUse( int width = 1, int height = 1, int corner = 0 );
		~BlockComponentSingleUse();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;

	private:
		int width_;
		int height_;
		int corner_;
};

#endif // BLOCK_COMPONENT_SINGLE_USE_H

