#pragma once

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentVerySteep final : public BlockComponent
{
	public:
		BlockComponentVerySteep( Direction::Horizontal direction, int height, int relative, int resistance, int extra_push );
		~BlockComponentVerySteep();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const Direction::Horizontal direction_;
		int height_;
		int relative_;
		int resistance_;
		int extra_push_;

		int getSlopeTopFromX( const Sprite& sprite, const Block& block ) const;
		void adjustSpritesSpeed( Sprite& sprite ) const;
		double getResistance() const;
};
