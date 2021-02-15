#pragma once

#include "block_component.hpp"
#include "card_suit.hpp"

class BlockComponentSuit final : public BlockComponent
{
	public:
		BlockComponentSuit( CardSuit suit );
		~BlockComponentSuit();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

    private:
        CardSuit suit_;
};

