#pragma once

#include "block_component.hpp"

class BlockComponentSetGravity final : public BlockComponent
{
	public:
		BlockComponentSetGravity( double value = -1.0 );
		~BlockComponentSetGravity();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

    private:
        const double value_;
};
