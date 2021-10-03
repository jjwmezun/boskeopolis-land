#pragma once

#include "sprite.hpp"

class ReticleSprite final : public Sprite
{
	public:
		ReticleSprite( int x, int y );
		~ReticleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class ReticleState
		{
			CHASING,
			STOPPING,
			SHOOTING
		};
		ReticleState state_;
		int timer_;
		char scratch_[3000];
};
