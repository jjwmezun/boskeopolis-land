#pragma once

#include "sprite.hpp"

class Block;

class SpikyFruitSprite : public Sprite
{
	public:
		SpikyFruitSprite( int x, int y, const Map& lvmap );
		~SpikyFruitSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		bool fallingBoughIsBack() const;

		bool connected_to_falling_bough_;
		Sprite* bough_;
};
