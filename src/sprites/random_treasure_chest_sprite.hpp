#pragma once

#include "treasure_chest_sprite.hpp"

class RandomTreasureChestSprite : public TreasureChestSprite
{
	public:
		RandomTreasureChestSprite( int x, int y );
		~RandomTreasureChestSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		enum class State
		{
			INIT,
			DORMANT,
			OPENING,
			ITEM_RISING,
			FINISHED
		};
		State state_;
		int id_;

		void debugChestTypeRandomness();
		void setRandomItemType();
};
