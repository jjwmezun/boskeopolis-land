#pragma once

#include "sprite.hpp"

class BabyMosesSprite final : public Sprite
{
	public:
		BabyMosesSprite( int x, int y );
		~BabyMosesSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        bool held_;
		bool throw_;
		int throw_speed_;
		Direction::Horizontal throw_dir_;
        char scratch_[3000];
};
