#pragma once

#include "treasure_chest_sprite.hpp"

class EventSystem;

class RandomTreasureChestSprite : public TreasureChestSprite
{
	public:
		RandomTreasureChestSprite( int x, int y, EventSystem& events );
		~RandomTreasureChestSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

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

		void setRandomItemType( EventSystem& events );
};
