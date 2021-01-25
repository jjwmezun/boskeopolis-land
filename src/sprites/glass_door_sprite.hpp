#pragma once

#include "sprite.hpp"

class GlassDoorSprite final : public Sprite
{
	public:
		GlassDoorSprite( int x, int y );
		~GlassDoorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int hits_;
};
