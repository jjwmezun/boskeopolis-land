#pragma once

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class SpriteSystem;

class DungeonEnemySprite : public Sprite
{
	public:
		DungeonEnemySprite( int x, int y, int layer, bool stationary = false );
		~DungeonEnemySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class State
		{
			WALK,
			SHOOT,
			CHANGE
		};

		void updateActions( SpriteSystem& sprites );
		void updateGraphics();
		void changeDirection();
		void shoot( SpriteSystem& sprites );
		void updateInvincibility();
		void hurt();
		bool invincibilityFlickerOff() const;

		bool stationary_;
		int walk_delay_;
		int walk_timer_;
		int hp_;
		State state_;
		State next_state_;
		TimerSimpleT<48, false> invincibility_timer_;
};
