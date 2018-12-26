#pragma once

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentVerySteep : public BlockComponent
{
	public:
		BlockComponentVerySteep( Direction::Horizontal direction, int height, int relative, int resistance, int extra_push );
		~BlockComponentVerySteep();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

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