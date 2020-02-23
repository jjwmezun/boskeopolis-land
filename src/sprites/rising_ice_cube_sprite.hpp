#pragma once

#include "sprite.hpp"

class RisingIceCubeSprite : public Sprite
{
	public:
		RisingIceCubeSprite( int x, int y, int height_limit = 8 );
		~RisingIceCubeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class State
		{
			PAUSED,
			RISING,
			FALLING
		};
		State state_;
		int height_limit_;
};