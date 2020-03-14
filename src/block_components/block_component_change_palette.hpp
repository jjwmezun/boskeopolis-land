#pragma once

#include "block_component.hpp"
#include "palette.hpp"

class BlockComponentChangePalette final : public BlockComponent
{
	public:
		BlockComponentChangePalette( const Palette& palette );
		~BlockComponentChangePalette();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const Palette palette_;
};