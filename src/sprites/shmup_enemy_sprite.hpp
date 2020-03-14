#pragma once

#include "sprite.hpp"

class Camera;

class ShmupEnemySprite final : public Sprite
{
	public:
		ShmupEnemySprite();
		~ShmupEnemySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool reset_;
		int shoot_timer_;
		int shoot_timer_target_;

		void doReset( const Camera& camera );
};
