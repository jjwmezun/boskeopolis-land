#pragma once

#include "sprite.hpp"

class Camera;

class ShmupEnemySprite : public Sprite
{
	public:
		ShmupEnemySprite();
		~ShmupEnemySprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		bool reset_;
		int shoot_timer_;
		int shoot_timer_target_;

		void doReset( const Camera& camera );
};
