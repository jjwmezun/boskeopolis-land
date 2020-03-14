#pragma once

#include "sprite.hpp"

class DungeonSwitchSprite final : public Sprite
{
	public:
		DungeonSwitchSprite( int x, int y );
		~DungeonSwitchSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool on_;
};
