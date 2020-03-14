#pragma once

#include "sprite.hpp"

class Block;

class SpikyFruitSprite final : public Sprite
{
	public:
		SpikyFruitSprite( int x, int y, const Map& lvmap );
		~SpikyFruitSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool fallingBoughIsBack() const;

		bool connected_to_falling_bough_;
		Sprite* bough_;
};
