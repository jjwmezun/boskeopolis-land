#ifndef BLOCK_COMPONENT_CHANGE_PALETTE_H
#define BLOCK_COMPONENT_CHANGE_PALETTE_H

#include "block_component.hpp"
#include "palette.hpp"

class BlockComponentChangePalette : public BlockComponent
{
	public:
		BlockComponentChangePalette( const Palette& palette );
		~BlockComponentChangePalette();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const Palette palette_;
};

#endif // BLOCK_COMPONENT_CHANGE_PALETTE_H

