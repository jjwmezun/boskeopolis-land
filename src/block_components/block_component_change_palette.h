#ifndef BLOCK_COMPONENT_CHANGE_PALETTE_H
#define BLOCK_COMPONENT_CHANGE_PALETTE_H

#include "block_component.h"
#include "palette.h"

class BlockComponentChangePalette : public BlockComponent
{
	public:
		BlockComponentChangePalette( const Palette& palette );
		~BlockComponentChangePalette();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;

	private:
		const Palette palette_;
};

#endif // BLOCK_COMPONENT_CHANGE_PALETTE_H

