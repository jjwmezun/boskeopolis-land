#pragma once

#include "sprite.hpp"

class HoppingZombieSprite final : public Sprite
{
	public:
		HoppingZombieSprite( int x, int y );
		~HoppingZombieSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int timer_;
        int target_time_;
		sdl2::SDLRect head_box_;
		sdl2::SDLRect hurt_box_;
        char scatch_[ 3000 ];
};
