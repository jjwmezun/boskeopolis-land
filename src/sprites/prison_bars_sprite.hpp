#pragma once

#include "sprite.hpp"

class PrisonBarsSprite final : public Sprite
{
	public:
		PrisonBarsSprite( int x, int y, int switch_number );
		~PrisonBarsSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int switch_number_;
};
