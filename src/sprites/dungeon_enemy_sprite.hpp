#pragma once

#include "sprite.hpp"

class SpriteSystem;

class DungeonEnemySprite : public Sprite
{
	public:
		DungeonEnemySprite( int x, int y, int layer );
		~DungeonEnemySprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

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

		int walk_delay_;
		int walk_timer_;
		State state_;
		State next_state_;
};
