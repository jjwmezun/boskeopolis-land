#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class CockroachSprite final : public Sprite
{
	public:
		CockroachSprite( int x, int y );
		~CockroachSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( LevelState& level_state ) override;
		void render( Camera& camera, bool priority = false ) override;
		void renderSuperPriority( Camera& camera ) override;

	private:
		int timer_;
};
