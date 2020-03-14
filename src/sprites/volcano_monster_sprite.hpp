#pragma once

#include "sprite.hpp"

class SpriteSystem;

class VolcanoMonsterSprite final : public Sprite
{
	public:
		VolcanoMonsterSprite( int x, int y );
		~VolcanoMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
		int next_spit_;
		int animation_timer_;

		void spawn( SpriteSystem& sprites );
};
