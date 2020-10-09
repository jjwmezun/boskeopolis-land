#pragma once

#include "sprite.hpp"

class ZombieSprite final : public Sprite
{
	public:
		ZombieSprite( int x, int y );
		~ZombieSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
		enum class ZState
		{
			RISING,
			MOVING,
			BEFORE_THROW,
			THROWING,
			PULLING

		};
		ZState state_;
		int timer_;
		int head_acceleration_x_;
		int head_vx_;
		sdl2::SDLRect head_;
        char scatch_[ 3000 ];
};
